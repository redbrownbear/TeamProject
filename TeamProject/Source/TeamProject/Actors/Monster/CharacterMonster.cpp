// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"
#include "Components/CapsuleComponent.h"

#include "Shakes/DefaultCameraShakeBase.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Data/MonsterTableRow.h"
#include "Data/ItemDataRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/DamageEvents.h"


// Sets default values
ACharacterMonster::ACharacterMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCanEverAffectNavigation(false);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACharacterMonster::OnBeginOverlap);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ACharacterMonster::OnEndOverlap);
	RootComponent = CapsuleComp;

	StatusComponent = CreateDefaultSubobject<UMonsterStatusComponent>(TEXT("StatusComponent"));
}

// Called when the game starts or when spawned
void ACharacterMonster::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionProfileName(CollisionProfileName::Monster);
	CapsuleComp->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetCharacterMonster(this);
		FSMComponent->BindHitEvent();
	}
	SetData(DataTableRowHandle);

	StatusComponent->OnDie.AddDynamic(this, &ACharacterMonster::OnDie);
}

// Called every frame
void ACharacterMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ACharacterMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAnimInstance* ACharacterMonster::GetAnimInstance() const
{
	return GetMonsterMesh()->GetAnimInstance();
}

UMonsterFSMComponent* ACharacterMonster::GetFSMComponent() const
{
	if (AMonsterAIController* MonsterAIController = Cast<AMonsterAIController>(GetController()))
	{
		if (UMonsterFSMComponent* MonsterFSMComponent = Cast<UMonsterFSMComponent>(MonsterAIController->GetComponentByClass(UMonsterFSMComponent::StaticClass())))
		{
			return MonsterFSMComponent;
		}
	}

	return nullptr;
}

void ACharacterMonster::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMonsterTableRow* Data = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MonsterData = Data;

	// 캡슐 컴포넌트 설정 (이미 ACharacter에 의해 생성되고 등록된 상태)
	UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();
	if (CapsuleComp)
	{
		//CapsuleComp->SetSphereRadius(MonsterData->CollisionSphereRadius); // SetCapsuleHalfHeight와 충돌 가능성 있음
		CapsuleComp->SetCollisionProfileName(CollisionProfileName::Monster);
		CapsuleComp->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		CapsuleComp->SetCapsuleHalfHeight(MonsterData->CapsuleHalfHeight);
		CapsuleComp->SetCapsuleRadius(MonsterData->CapsuleRadius);
	}

	// 스켈레탈 메쉬 컴포넌트 설정 (이미 ACharacter에 의해 생성되고 등록된 상태)
	USkeletalMeshComponent* MeshComp = GetMesh(); // GetMesh()는 ACharacter의 USkeletalMeshComponent를 반환합니다.
	if (MeshComp) // MeshComp가 유효한지 확인
	{
		MeshComp->SetSkeletalMesh(MonsterData->SkeletalMesh);
		MeshComp->SetAnimClass(MonsterData->AnimClass);
		MeshComp->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
		FVector RelativeLocation = MonsterData->MeshTransform.GetLocation();
		RelativeLocation += FVector(0.0, 0.0, -MonsterData->CapsuleHalfHeight);
		MeshComp->SetRelativeLocation(RelativeLocation);
	}


	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp) // MovementComp가 유효한지 확인
	{
		MovementComp->MaxWalkSpeed = MonsterData->WalkMovementMaxSpeed;
	}

	AIControllerClass = MonsterData->AIControllerClass;

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetMonsterGroupType(MonsterData->eMonsterGroupType);
	}

	StatusComponent->SetMaxHP(MonsterData->MaxHP);

	if (!(MonsterData->MeleeWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* MeleeWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			MeleeWeapon->SetDataWithHandle(MonsterData->MeleeWeaponTableRowHandle);
			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FVector Scale = MeleeWeapon->GetActorScale3D();

				if (FSMComponent->IsA<ULynelFSMComponent>())
				{
					Scale *= 2.f;
				}

				MeleeWeapon->SetActorScale3D(Scale);
				MeleeWeapon->AttachToMonster(this, Monster_SocketName::Pod_Melee);
				MeleeWeapon->FinishSpawning(FTransform::Identity);

				FSMComponent->SetMeleeWeapon(MeleeWeapon);
				FSMComponent->SheathMeleeWeapon();
			}
		}
	}


	if (!(MonsterData->BowWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* BowWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			BowWeapon->SetDataWithHandle(MonsterData->BowWeaponTableRowHandle);
			const FVector Scale = BowWeapon->GetActorScale3D() * 2.f;
			BowWeapon->SetActorScale3D(Scale);
			BowWeapon->AttachToMonster(this, Monster_SocketName::Pod_Bow);
			BowWeapon->FinishSpawning(FTransform::Identity);

			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FSMComponent->SetBowWeapon(BowWeapon);
				FSMComponent->SheathBowWeapon();
			}
		}
	}

	for (USphereComponent* ExistingCollider : AdditionalColliders)
	{
		if (ExistingCollider && ExistingCollider->IsValidLowLevelFast()) // 유효성 확인
		{
			ExistingCollider->DestroyComponent(); // 컴포넌트 파괴
		}
	}
	AdditionalColliders.Empty(); // 배열 비우기

	if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox") || DataTableRowHandle.RowName.ToString() == TEXT("Lynel"))
	{
		USkeletalMeshComponent* SkeletalmeshComp = GetMesh();

		if (!SkeletalmeshComp) 
		{
			UE_LOG(LogTemp, Warning, TEXT("SetData: SkeletalMeshComponent is null. Cannot attach colliders."));
			return; 
		}

		// Hinox와 Lynel 모두를 처리하는 단일 루프
		int32 NumCollidersToCreate = 0;
		if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox"))
		{
			NumCollidersToCreate = static_cast<int32>(EAdditionalCollider::End);
		}
		else if (DataTableRowHandle.RowName.ToString() == TEXT("Lynel"))
		{
			// Lynel의 경우 EAdditionalCollider::Chin까지만 사용하도록 설정
			NumCollidersToCreate = static_cast<int32>(EAdditionalCollider::Chin) + 1;
		}

		AdditionalColliders.Reserve(NumCollidersToCreate); 

		for (int32 i = 0; i < NumCollidersToCreate; ++i)
		{
			FString ComponentName = FString::Printf(TEXT("AdditionalCollider_%d"), i);
			USphereComponent* NewSphereCollider = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), FName(*ComponentName));

			if (NewSphereCollider)
			{
				NewSphereCollider->RegisterComponent(); 

				AdditionalColliders.Add(NewSphereCollider);

				NewSphereCollider->SetCanEverAffectNavigation(false);
				NewSphereCollider->SetCollisionProfileName(CollisionProfileName::Monster); // 몬스터 피격 판정에 적합한 프로파일

				if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox"))
				{
					if (static_cast<EAdditionalCollider>(i) == EAdditionalCollider::Eye_Ball)
					{
						NewSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnEyeBeginOverlap);
					}
					else
					{
						NewSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
					}
				}
				else if (DataTableRowHandle.RowName.ToString() == TEXT("Lynel"))
				{
					if (static_cast<EAdditionalCollider>(i) == EAdditionalCollider::Chin)
					{
						NewSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnEyeBeginOverlap);
					}
					else
					{
						NewSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
					}
				}
				FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

				FName AttachSocketName = NAME_None; // 기본은 메시의 루트

				// 몬스터 타입 및 콜리더 인덱스에 따른 소켓 이름 및 크기 설정
				if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox"))
				{
					switch (static_cast<EAdditionalCollider>(i))
					{
					case EAdditionalCollider::Chin:
						AttachSocketName = Monster_SocketName::Chin;
						NewSphereCollider->SetSphereRadius(75.0f);
						break;
					case EAdditionalCollider::Center_1:
						AttachSocketName = Monster_SocketName::Center_1;
						NewSphereCollider->SetSphereRadius(150.f);
						break;
					case EAdditionalCollider::Center_2:
						AttachSocketName = Monster_SocketName::Center_2;
						NewSphereCollider->SetSphereRadius(150.f);
						break;
					case EAdditionalCollider::Leg_1_R:
						AttachSocketName = Monster_SocketName::Leg_1_R;
						NewSphereCollider->SetSphereRadius(50.f);
						break;
					case EAdditionalCollider::Leg_2_R:
						AttachSocketName = Monster_SocketName::Leg_2_R;
						NewSphereCollider->SetSphereRadius(50.f);
						break;
					case EAdditionalCollider::Leg_1_L:
						AttachSocketName = Monster_SocketName::Leg_1_L;
						NewSphereCollider->SetSphereRadius(50.f);
						break;
					case EAdditionalCollider::Leg_2_L:
						AttachSocketName = Monster_SocketName::Leg_2_L;
						NewSphereCollider->SetSphereRadius(50.f);
						break;
					case EAdditionalCollider::Toe_R:
						AttachSocketName = Monster_SocketName::Toe_R;
						NewSphereCollider->SetSphereRadius(50.f);
						break;
					case EAdditionalCollider::Toe_L:
						AttachSocketName = Monster_SocketName::Toe_L;
						NewSphereCollider->SetSphereRadius(50.f);
						break;
					case EAdditionalCollider::Eye_Ball:
						AttachSocketName = Monster_SocketName::EyeBall;
						NewSphereCollider->SetSphereRadius(70.f);
						break;
					default:
						UE_LOG(LogTemp, Error, TEXT("ACharacterMonster::SetData // Hinox: Unexpected AdditionalCollider index: %d"), i);
						// check(false); // 릴리스 빌드에 영향을 주므로 주의하여 사용
						break;
					}
				}
				else if (DataTableRowHandle.RowName.ToString() == TEXT("Lynel"))
				{
					switch (static_cast<EAdditionalCollider>(i))
					{
					case EAdditionalCollider::Chin:
						AttachSocketName = Monster_SocketName::Chin;
						NewSphereCollider->SetSphereRadius(50.0f);
						break;
					default:
						UE_LOG(LogTemp, Error, TEXT("ACharacterMonster::SetData // Lynel: Unexpected AdditionalCollider index: %d"), i);
						// check(false);
						break;
					}
				}

				// 실제 부착 수행
				NewSphereCollider->AttachToComponent(SkeletalmeshComp, AttachRules, AttachSocketName);
				NewSphereCollider->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;

			}
		}
	}

	if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox"))
	{
		DefaultCameraShakeBase = NewObject<UDefaultCameraShakeBase>(this, UDefaultCameraShakeBase::StaticClass(), TEXT("DefaultCameraShakeBase"));
	}
}
void ACharacterMonster::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		//UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();
		//CapsuleComp->DestroyComponent();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void ACharacterMonster::PostLoad()
{
	Super::PostLoad();
}

void ACharacterMonster::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void ACharacterMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACharacterMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

void ACharacterMonster::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		if (ProjectileName::Monster_PlayerAlert == Projectile->GetProjectileName())
		{
			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FSMComponent->ChangeState(EMonsterState::FindWeapon);
				if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn()))
					{
						FSMComponent->SetPlayer(Player);
					}
					else
					{
						check(false);
					}
				}
				else
				{
					check(false);
				}
			}
		}
		else if (ProjectileName::Player_Arrow == Projectile->GetProjectileName())
		{
			// float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser
			FDamageEvent DamageEvent;

			if (UWorld* World = GetWorld())
			{
				if (AController* PlayerController = World->GetFirstPlayerController())
				{
					if (AActor* Player = PlayerController->GetPawn())
					{
						IMonsterInterface::TakeDamage(Projectile->GetDamage(), DamageEvent, PlayerController, Player);
					}
				}
			}
		}
		else if (ProjectileName::Monster_AB_KogaStone == Projectile->GetProjectileName()
			|| ProjectileName::Monster_AB_KogaStoneBig == Projectile->GetProjectileName()
			)		
		{
			FDamageEvent DamageEvent;
			if (UWorld* World = GetWorld())
			{
				if (AController* PlayerController = World->GetFirstPlayerController())
				{
					if (AActor* Player = Controller->GetPawn())
					{
						IMonsterInterface::TakeDamage(Projectile->GetDamage(), DamageEvent, GetController(), this);
					}
				}
			}
		}
	}
}
void ACharacterMonster::OnEyeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		if (ProjectileName::Player_Arrow == Projectile->GetProjectileName())
		{
			// float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser
			FDamageEvent DamageEvent;

			if (UWorld* World = GetWorld())
			{
				if (AController* PlayerController = World->GetFirstPlayerController())
				{
					if (AActor* Player = PlayerController->GetPawn())
					{
						IMonsterInterface::TakeDamage(Projectile->GetDamage(), DamageEvent, PlayerController, Player, 1);
					}
				}
			}
		}
	}
}
void ACharacterMonster::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

APatrolPath* ACharacterMonster::GetPatrolPath() const
{
	return PatrolPath;
}

ACampFire* ACharacterMonster::GetCampFire() const
{
	return CampFire;
}

void ACharacterMonster::SetSpeedWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = MonsterData->WalkMovementMaxSpeed;
}

void ACharacterMonster::SetSpeedRun()
{
	GetCharacterMovement()->MaxWalkSpeed = MonsterData->RunMovementMaxSpeed;
}

void ACharacterMonster::OnDie()
{
	IMonsterInterface::OnDie();
}

void ACharacterMonster::OnDeadEnd()
{
	// it would call by AnimNotify 
	IMonsterInterface::OnDeadEnd();
}
