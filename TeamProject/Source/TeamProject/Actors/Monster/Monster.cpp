// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/Monster.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"

#include "Components/SphereComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Components/FSMComponent/MonsterFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Data/MonsterTableRow.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMonster::AMonster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UAdvancedFloatingPawnMovement>(TEXT("MovementComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	//CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);

	CollisionComponent->SetCanEverAffectNavigation(false);
	RootComponent = CollisionComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	FRotator NewRotator = FRotator(0.0, 0.0, 0.0);
	SkeletalMeshComponent->SetWorldRotation(NewRotator.Quaternion());


	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfig_Sight->SightRadius = MONSTER_AISENSECONFIG_SIGHT_SIGHTRADIUS;
	AISenseConfig_Sight->LoseSightRadius = MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS_PERIPHERAL_VISIONANGLEDEGREES;
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);

	StatusComponent = CreateDefaultSubobject<UMonsterStatusComponent>(TEXT("StatusComponent"));
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	SetData(DataTableRowHandle);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	//if (AMonsterAIController* MonsterAIController = Cast<AMonsterAIController>(GetController()))
	//{
	//	if (UMonsterFSMComponent* MonsterFSMComponent = Cast<UMonsterFSMComponent>(MonsterAIController->GetComponentByClass(UMonsterFSMComponent::StaticClass())))
	//	{
	//		if (FMath::IsNearlyZero(Speed))
	//		{
	//			MonsterFSMComponent->SetMonsterMovementState(EMonsterMovementState::Idle);
	//		}
	//		else if (FMath::IsNearlyEqual(Speed, MonsterData->WalkMovementMaxSpeed))
	//		{
	//			MonsterFSMComponent->SetMonsterMovementState(EMonsterMovementState::Walk);
	//		}
	//		else if (FMath::IsNearlyEqual(Speed, MonsterData->RunMovementMaxSpeed))
	//		{
	//			MonsterFSMComponent->SetMonsterMovementState(EMonsterMovementState::Run);
	//		}
	//	}
	//}
}

UMonsterFSMComponent* AMonster::GetFSMComponent() const
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

void AMonster::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMonsterTableRow* Data = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MonsterData = Data;

	SkeletalMeshComponent->SetSkeletalMesh(MonsterData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(MonsterData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -MonsterData->CollisionSphereRadius));

	CollisionComponent->SetSphereRadius(MonsterData->CollisionSphereRadius);
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	CollisionComponent->RegisterComponent();

	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;

	AIControllerClass = MonsterData->AIControllerClass;
}

void AMonster::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		CollisionComponent->DestroyComponent();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void AMonster::PostLoad()
{
	Super::PostLoad();
}

void AMonster::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}
