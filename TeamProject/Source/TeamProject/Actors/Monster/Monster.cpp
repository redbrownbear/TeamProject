// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/Monster.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Data/MonsterTableRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonster::AMonster()
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

void AMonster::PlayMontage(EMonsterMontage _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	if (!MonsterData) return;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case EMonsterMontage::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case EMonsterMontage::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case EMonsterMontage::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case EMonsterMontage::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case EMonsterMontage::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case EMonsterMontage::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case EMonsterMontage::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case EMonsterMontage::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case EMonsterMontage::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case EMonsterMontage::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case EMonsterMontage::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case EMonsterMontage::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case EMonsterMontage::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case EMonsterMontage::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case EMonsterMontage::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case EMonsterMontage::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case EMonsterMontage::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case EMonsterMontage::SIGNAL_START:
		TempAnimMontage = MonsterData->SignalStartMontage;
		break;
	case EMonsterMontage::SIGNAL_END:
		TempAnimMontage = MonsterData->SignalEndMontage;
		break;
	case EMonsterMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
	{
		check(false);
	}
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
	else
	{
		int a = 0;
	}
}

bool AMonster::IsMontage(EMonsterMontage _InEnum)
{
	if (!MonsterData) return false;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case EMonsterMontage::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case EMonsterMontage::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case EMonsterMontage::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case EMonsterMontage::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case EMonsterMontage::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case EMonsterMontage::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case EMonsterMontage::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case EMonsterMontage::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case EMonsterMontage::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case EMonsterMontage::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case EMonsterMontage::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case EMonsterMontage::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case EMonsterMontage::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case EMonsterMontage::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case EMonsterMontage::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case EMonsterMontage::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case EMonsterMontage::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case EMonsterMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return TempAnimMontage ? true : false;
}

bool AMonster::IsPlayingMontage(EMonsterMontage _InEnum)
{
	if (!MonsterData) return false;
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case EMonsterMontage::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case EMonsterMontage::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case EMonsterMontage::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case EMonsterMontage::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case EMonsterMontage::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case EMonsterMontage::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case EMonsterMontage::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case EMonsterMontage::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case EMonsterMontage::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case EMonsterMontage::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case EMonsterMontage::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case EMonsterMontage::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case EMonsterMontage::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case EMonsterMontage::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case EMonsterMontage::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case EMonsterMontage::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case EMonsterMontage::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case EMonsterMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return AnimInstance->Montage_IsPlaying(nullptr);
}

void AMonster::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AMonster::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AMonster::SetSpeedWalk()
{
	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;
}

void AMonster::SetSpeedRun()
{
	MovementComponent->MaxSpeed = MonsterData->RunMovementMaxSpeed;
}