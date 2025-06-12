// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"
#include "Actors/Object/ProjectileMetalActor.h"

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

	UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(CollisionProfileName::Monster);
		CapsuleComp->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		CapsuleComp->SetCapsuleHalfHeight(MonsterData->CapsuleHalfHeight);
		CapsuleComp->SetCapsuleRadius(MonsterData->CapsuleRadius);
	}

	USkeletalMeshComponent* MeshComp = GetMesh(); // GetMesh() returns ACharacter's USkeletalMeshComponent.
	if (MeshComp) // Make sure MeshComp is Valid
	{
		MeshComp->SetSkeletalMesh(MonsterData->SkeletalMesh);
		MeshComp->SetAnimClass(MonsterData->AnimClass);
		MeshComp->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
		FVector RelativeLocation = MonsterData->MeshTransform.GetLocation();
		RelativeLocation += FVector(0.0, 0.0, -MonsterData->CapsuleHalfHeight);
		MeshComp->SetRelativeLocation(RelativeLocation);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	for (USphereComponent* ExistingCollider : AdditionalColliders)
	{
		if (ExistingCollider && ExistingCollider->IsValidLowLevelFast()) 
		{
			ExistingCollider->DestroyComponent(); 
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



	int32 MaterialSlotIndex = -1;
	if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox"))
	{
		MaterialSlotIndex = 4;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("Moriblin_TreasureBox"))
	{
		MaterialSlotIndex = 1;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("AssasinLeader"))
	{
		MaterialSlotIndex = 0;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("AssasinBoss"))
	{
		MaterialSlotIndex = 1;
	}

	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

	if (MaterialSlotIndex != -1)
	{
		UMaterialInterface* CurrentMaterialOnMesh = SkeletalMeshComponent->GetMaterial(MaterialSlotIndex);

		if (!CurrentMaterialOnMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetData: No material found at slot %d. Cannot proceed with DynamicMaterialInstance setup."), MaterialSlotIndex);
			DynamicMaterialInstance = nullptr; // 기존 포인터도 혹시 모르니 null로
			return;
		}

		DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentMaterialOnMesh);

		if (!DynamicMaterialInstance)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterialOnMesh, this);

			if (DynamicMaterialInstance)
			{
				SkeletalMeshComponent->SetMaterial(MaterialSlotIndex, DynamicMaterialInstance);
				UE_LOG(LogTemp, Log, TEXT("SetData: Created and assigned new DynamicMaterialInstance (%s) to slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SetData: Failed to create DynamicMaterialInstance using parent %s!"), *CurrentMaterialOnMesh->GetName());
				DynamicMaterialInstance = nullptr;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SetData: Re-using existing DynamicMaterialInstance (%s) at slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SetData: No specific material slot handling for %s."), *DataTableRowHandle.RowName.ToString());
		DynamicMaterialInstance = nullptr;
	}

	AddBaseColor(FVector(0.0, 0.0, 0.0));
}
void ACharacterMonster::SetData(const FName& MonsterName)
{
	static UDataTable* MonsterDataTable = nullptr;
	if (!MonsterDataTable)
	{
		MonsterDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/MonsterData/DT_Monster.DT_Monster'"));
		check(MonsterDataTable);
	}

	if (!MonsterDataTable->GetRowMap().Find(MonsterName)) { ensure(false); return; }
	DataTableRowHandle.DataTable = MonsterDataTable;
	DataTableRowHandle.RowName = MonsterName;
	MonsterData = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());


	UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(CollisionProfileName::Monster);
		CapsuleComp->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		CapsuleComp->SetCapsuleHalfHeight(MonsterData->CapsuleHalfHeight);
		CapsuleComp->SetCapsuleRadius(MonsterData->CapsuleRadius);
	}

	USkeletalMeshComponent* MeshComp = GetMesh(); // GetMesh() returns ACharacter's USkeletalMeshComponent.
	if (MeshComp) // Make sure MeshComp is Valid
	{
		MeshComp->SetSkeletalMesh(MonsterData->SkeletalMesh);
		MeshComp->SetAnimClass(MonsterData->AnimClass);
		MeshComp->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
		FVector RelativeLocation = MonsterData->MeshTransform.GetLocation();
		RelativeLocation += FVector(0.0, 0.0, -MonsterData->CapsuleHalfHeight);
		MeshComp->SetRelativeLocation(RelativeLocation);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	for (USphereComponent* ExistingCollider : AdditionalColliders)
	{
		if (ExistingCollider && ExistingCollider->IsValidLowLevelFast())
		{
			ExistingCollider->DestroyComponent();
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



	int32 MaterialSlotIndex = -1;
	if (DataTableRowHandle.RowName.ToString() == TEXT("Hinox"))
	{
		MaterialSlotIndex = 4;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("Moriblin_TreasureBox"))
	{
		MaterialSlotIndex = 1;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("AssasinLeader"))
	{
		MaterialSlotIndex = 0;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("AssasinBoss"))
	{
		MaterialSlotIndex = 1;
	}

	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

	if (MaterialSlotIndex != -1)
	{
		UMaterialInterface* CurrentMaterialOnMesh = SkeletalMeshComponent->GetMaterial(MaterialSlotIndex);

		if (!CurrentMaterialOnMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetData: No material found at slot %d. Cannot proceed with DynamicMaterialInstance setup."), MaterialSlotIndex);
			DynamicMaterialInstance = nullptr; // 기존 포인터도 혹시 모르니 null로
			return;
		}

		DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentMaterialOnMesh);

		if (!DynamicMaterialInstance)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterialOnMesh, this);

			if (DynamicMaterialInstance)
			{
				SkeletalMeshComponent->SetMaterial(MaterialSlotIndex, DynamicMaterialInstance);
				UE_LOG(LogTemp, Log, TEXT("SetData: Created and assigned new DynamicMaterialInstance (%s) to slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SetData: Failed to create DynamicMaterialInstance using parent %s!"), *CurrentMaterialOnMesh->GetName());
				DynamicMaterialInstance = nullptr;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SetData: Re-using existing DynamicMaterialInstance (%s) at slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SetData: No specific material slot handling for %s."), *DataTableRowHandle.RowName.ToString());
		DynamicMaterialInstance = nullptr;
	}

	AddBaseColor(FVector(0.0, 0.0, 0.0));
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
		else if (ProjectileName::Player_Arrow == Projectile->GetProjectileName()
			|| ProjectileName::Player_FireArrow == Projectile->GetProjectileName()
			)
		{
			// float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser
			FDamageEvent DamageEvent;

			if (UWorld* World = GetWorld())
			{
				if (AController* PlayerController = World->GetFirstPlayerController())
				{
					if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn()))
					{
						GetFSMComponent()->SetPlayer(Player);
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
	else if (AProjectileMetalActor* ProjectileMetalActor = Cast<AProjectileMetalActor>(OtherActor))
	{
		FDamageEvent DamageEvent;
		if (UWorld* World = GetWorld())
		{
			if (AController* PlayerController = World->GetFirstPlayerController())
			{
				if (AActor* Player = Controller->GetPawn())
				{
					IMonsterInterface::TakeDamage(ProjectileMetalActor->GetDamage(), DamageEvent, GetController(), this);
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

UMaterialInterface* ACharacterMonster::GetMaterialInterface()
{
	return MaterialInterface;
}

UMaterialInstanceDynamic* ACharacterMonster::GetDynamicMaterialInstance()
{
	return DynamicMaterialInstance;
}

void ACharacterMonster::AddBaseColor(FVector InColor)
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue(TEXT("AddColor"), InColor);
	}
}
