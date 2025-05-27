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
#include "Components/CapsuleComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Data/MonsterTableRow.h"
#include "Data/ItemDataRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	SetData(DataTableRowHandle);

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		if (ULynelFSMComponent* LynelFSMComponent = Cast<ULynelFSMComponent>(FSMComponent))
		{
			LynelFSMComponent->SetCharacterMonster(this);
		}
	}

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

	UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();

	if (CapsuleComp)
	{
		//CapsuleComp->SetSphereRadius(MonsterData->CollisionSphereRadius);
		CapsuleComp->SetCollisionProfileName(CollisionProfileName::Monster);
		CapsuleComp->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		CapsuleComp->SetCapsuleHalfHeight(MonsterData->CollisionSphereRadius);
		CapsuleComp->RegisterComponent();
	}

	USkeletalMeshComponent* MeshComp = GetComponentByClass<USkeletalMeshComponent>();
	MeshComp->SetSkeletalMesh(MonsterData->SkeletalMesh);
	MeshComp->SetAnimClass(MonsterData->AnimClass);
	MeshComp->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());

	MeshComp->SetRelativeLocation(FVector(0.0, 0.0, -MonsterData->CollisionSphereRadius));



	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->MaxWalkSpeed = MonsterData->WalkMovementMaxSpeed;

	AIControllerClass = MonsterData->AIControllerClass;

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetMonsterGroupType(MonsterData->eMonsterGroupType);
	}

	if (!(MonsterData->MeleeWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* MeleeWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			MeleeWeapon->SetDataWithHandle(MonsterData->MeleeWeaponTableRowHandle);
			const FVector Scale = MeleeWeapon->GetActorScale3D() * 2.f;
			MeleeWeapon->SetActorScale3D(Scale);
			MeleeWeapon->AttachToMonster(this, Monster_SocketName::Pod_Melee);
			MeleeWeapon->FinishSpawning(FTransform::Identity);

			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
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
