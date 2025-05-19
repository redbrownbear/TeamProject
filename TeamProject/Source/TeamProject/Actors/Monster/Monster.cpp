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

void AMonster::PlayMontage(MONSTER_MONTAGE _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	if (!MonsterData) return;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case MONSTER_MONTAGE::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case MONSTER_MONTAGE::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case MONSTER_MONTAGE::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case MONSTER_MONTAGE::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case MONSTER_MONTAGE::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case MONSTER_MONTAGE::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case MONSTER_MONTAGE::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case MONSTER_MONTAGE::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case MONSTER_MONTAGE::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case MONSTER_MONTAGE::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case MONSTER_MONTAGE::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case MONSTER_MONTAGE::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case MONSTER_MONTAGE::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	if (TempAnimMontage && !AnimInstance->Montage_IsPlaying(TempAnimMontage))
	{
		if (bIsLoop)
		{
			AnimInstance->Montage_Play(TempAnimMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
		else
		{
			AnimInstance->Montage_Play(TempAnimMontage);
		}
	}
}

bool AMonster::IsMontage(MONSTER_MONTAGE _InEnum)
{
	if (!MonsterData) return false;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case MONSTER_MONTAGE::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case MONSTER_MONTAGE::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case MONSTER_MONTAGE::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case MONSTER_MONTAGE::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case MONSTER_MONTAGE::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case MONSTER_MONTAGE::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case MONSTER_MONTAGE::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case MONSTER_MONTAGE::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case MONSTER_MONTAGE::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case MONSTER_MONTAGE::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case MONSTER_MONTAGE::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case MONSTER_MONTAGE::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case MONSTER_MONTAGE::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return TempAnimMontage ? true : false;
}

bool AMonster::IsPlayingMontage(MONSTER_MONTAGE _InEnum)
{
	if (!MonsterData) return false;
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case MONSTER_MONTAGE::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case MONSTER_MONTAGE::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case MONSTER_MONTAGE::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case MONSTER_MONTAGE::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case MONSTER_MONTAGE::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case MONSTER_MONTAGE::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case MONSTER_MONTAGE::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case MONSTER_MONTAGE::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case MONSTER_MONTAGE::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case MONSTER_MONTAGE::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case MONSTER_MONTAGE::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case MONSTER_MONTAGE::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case MONSTER_MONTAGE::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case MONSTER_MONTAGE::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return AnimInstance->Montage_IsPlaying(nullptr);
}
