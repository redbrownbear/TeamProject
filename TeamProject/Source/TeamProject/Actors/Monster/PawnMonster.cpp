// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/PawnMonster.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Data/MonsterTableRow.h"
#include "Data/ItemDataRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APawnMonster::APawnMonster()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UAdvancedFloatingPawnMovement>(TEXT("MovementComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	RootComponent = CollisionComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	FRotator NewRotator = FRotator(0.0, 0.0, 0.0);
	SkeletalMeshComponent->SetWorldRotation(NewRotator.Quaternion());

	StatusComponent = CreateDefaultSubobject<UMonsterStatusComponent>(TEXT("StatusComponent"));
}

// Called when the game starts or when spawned
void APawnMonster::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	SetData(DataTableRowHandle);

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetPawnMonster(this);
		FSMComponent->BindHitEvent();
	}

	StatusComponent->OnDie.AddDynamic(this, &ThisClass::OnDie);
}

// Called every frame
void APawnMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

}

UMonsterFSMComponent* APawnMonster::GetFSMComponent() const
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

void APawnMonster::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMonsterTableRow* Data = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MonsterData = Data;

	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(MonsterData->CollisionSphereRadius);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
		CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		CollisionComponent->RegisterComponent();
	}

	SkeletalMeshComponent->SetSkeletalMesh(MonsterData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(MonsterData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());

	// 모리블린 전용 오프셋
	if (TEXT("Moriblin_Patrol") == DataTableRowHandle.RowName.ToString()
		|| TEXT("Moriblin_TreasureBox") == DataTableRowHandle.RowName.ToString())
	{
		SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -1.5f * MonsterData->CollisionSphereRadius));
	}
	else
	{
		SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -MonsterData->CollisionSphereRadius));
	}




	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;

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

void APawnMonster::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		//CollisionComponent->DestroyComponent();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void APawnMonster::PostLoad()
{
	Super::PostLoad();
}

void APawnMonster::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void APawnMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APawnMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

void APawnMonster::SetSpeedWalk()
{
	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;
}

void APawnMonster::SetSpeedRun()
{
	MovementComponent->MaxSpeed = MonsterData->RunMovementMaxSpeed;
}

void APawnMonster::OnDie()
{
	IMonsterInterface::OnDie();
	// Maybe there's no Dead Montage;
	OnDeadEnd();
}

void APawnMonster::OnDeadEnd()
{
	IMonsterInterface::OnDeadEnd();
}

UAnimInstance* APawnMonster::GetAnimInstance() const
{
	return GetMonsterMesh()->GetAnimInstance();
}

void APawnMonster::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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


void APawnMonster::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

APatrolPath* APawnMonster::GetPatrolPath() const
{
	return PatrolPath;
}

ACampFire* APawnMonster::GetCampFire() const
{
	return CampFire;
}
