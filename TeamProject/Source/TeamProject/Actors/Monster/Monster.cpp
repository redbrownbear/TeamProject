// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/Monster.h"
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
	case EMonsterMontage::APPEAR:
		TempAnimMontage = MonsterData->APPEAR;
		break;
	case EMonsterMontage::BOW_UPPER_START:
		TempAnimMontage = MonsterData->BOW_UPPER_START;
		break;
	case EMonsterMontage::BOW_UPPER_END:
		TempAnimMontage = MonsterData->BOW_UPPER_END;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_END;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_START:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_START;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_END:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_END;
		break;
	case EMonsterMontage::ATTACK_FIRE_START:
		TempAnimMontage = MonsterData->ATTACK_FIRE_START;
		break;
	case EMonsterMontage::ATTACK_FIRE:
		TempAnimMontage = MonsterData->ATTACK_FIRE;
		break;
	case EMonsterMontage::ATTACK_FIRE_END:
		TempAnimMontage = MonsterData->ATTACK_FIRE_END;
		break;
	case EMonsterMontage::ATTACK_HORN_START:
		TempAnimMontage = MonsterData->ATTACK_HORN_START;
		break;
	case EMonsterMontage::ATTACK_HORN_END:
		TempAnimMontage = MonsterData->ATTACK_HORN_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_END;
		break;
	case EMonsterMontage::REBOUND:
		TempAnimMontage = MonsterData->REBOUND;
		break;
	case EMonsterMontage::RODEO_START:
		TempAnimMontage = MonsterData->RODEO_START;
		break;
	case EMonsterMontage::RODEO_END:
		TempAnimMontage = MonsterData->RODEO_END;
		break;
	case EMonsterMontage::STUN_START:
		TempAnimMontage = MonsterData->STUN_START;
		break;
	case EMonsterMontage::STUN_END:
		TempAnimMontage = MonsterData->STUN_END;
		break;
	case EMonsterMontage::TURN_180_L:
		TempAnimMontage = MonsterData->TURN_180_L;
		break;
	case EMonsterMontage::TURN_180_R:
		TempAnimMontage = MonsterData->TURN_180_R;
		break;
	case EMonsterMontage::DRAW_BOW:
		TempAnimMontage = MonsterData->DRAW_BOW;
		break;
	case EMonsterMontage::SHEATH_BOW:
		TempAnimMontage = MonsterData->SHEATH_BOW;
		break;
	case EMonsterMontage::BOW_TO_SWORD:
		TempAnimMontage = MonsterData->BOW_TO_SWORD;
		break;
	case EMonsterMontage::SWORD_TO_BOW:
		TempAnimMontage = MonsterData->SWORD_TO_BOW;
		break;
	case EMonsterMontage::DRAW_LSWORD:
		TempAnimMontage = MonsterData->DRAW_LSWORD;
		break;
	case EMonsterMontage::SHEATH_LSWORD:
		TempAnimMontage = MonsterData->SHEATH_LSWORD;
	break;	
	case EMonsterMontage::END:
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
	case EMonsterMontage::SIGNAL_START:
		TempAnimMontage = MonsterData->SignalStartMontage;
		break;
	case EMonsterMontage::SIGNAL_END:
		TempAnimMontage = MonsterData->SignalEndMontage;
		break;
	case EMonsterMontage::APPEAR:
		TempAnimMontage = MonsterData->APPEAR;
		break;
	case EMonsterMontage::BOW_UPPER_START:
		TempAnimMontage = MonsterData->BOW_UPPER_START;
		break;
	case EMonsterMontage::BOW_UPPER_END:
		TempAnimMontage = MonsterData->BOW_UPPER_END;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_END;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_START:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_START;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_END:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_END;
		break;
	case EMonsterMontage::ATTACK_FIRE_START:
		TempAnimMontage = MonsterData->ATTACK_FIRE_START;
		break;
	case EMonsterMontage::ATTACK_FIRE:
		TempAnimMontage = MonsterData->ATTACK_FIRE;
		break;
	case EMonsterMontage::ATTACK_FIRE_END:
		TempAnimMontage = MonsterData->ATTACK_FIRE_END;
		break;
	case EMonsterMontage::ATTACK_HORN_START:
		TempAnimMontage = MonsterData->ATTACK_HORN_START;
		break;
	case EMonsterMontage::ATTACK_HORN_END:
		TempAnimMontage = MonsterData->ATTACK_HORN_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_END;
		break;
	case EMonsterMontage::REBOUND:
		TempAnimMontage = MonsterData->REBOUND;
		break;
	case EMonsterMontage::RODEO_START:
		TempAnimMontage = MonsterData->RODEO_START;
		break;
	case EMonsterMontage::RODEO_END:
		TempAnimMontage = MonsterData->RODEO_END;
		break;
	case EMonsterMontage::STUN_START:
		TempAnimMontage = MonsterData->STUN_START;
		break;
	case EMonsterMontage::STUN_END:
		TempAnimMontage = MonsterData->STUN_END;
		break;
	case EMonsterMontage::TURN_180_L:
		TempAnimMontage = MonsterData->TURN_180_L;
		break;
	case EMonsterMontage::TURN_180_R:
		TempAnimMontage = MonsterData->TURN_180_R;
		break;
	case EMonsterMontage::DRAW_BOW:
		TempAnimMontage = MonsterData->DRAW_BOW;
		break;
	case EMonsterMontage::SHEATH_BOW:
		TempAnimMontage = MonsterData->SHEATH_BOW;
		break;
	case EMonsterMontage::BOW_TO_SWORD:
		TempAnimMontage = MonsterData->BOW_TO_SWORD;
		break;
	case EMonsterMontage::SWORD_TO_BOW:
		TempAnimMontage = MonsterData->SWORD_TO_BOW;
		break;
	case EMonsterMontage::DRAW_LSWORD:
		TempAnimMontage = MonsterData->DRAW_LSWORD;
		break;
	case EMonsterMontage::SHEATH_LSWORD:
		TempAnimMontage = MonsterData->SHEATH_LSWORD;
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
	case EMonsterMontage::SIGNAL_START:
		TempAnimMontage = MonsterData->SignalStartMontage;
		break;
	case EMonsterMontage::SIGNAL_END:
		TempAnimMontage = MonsterData->SignalEndMontage;
		break;
	case EMonsterMontage::APPEAR:
		TempAnimMontage = MonsterData->APPEAR;
		break;
	case EMonsterMontage::BOW_UPPER_START:
		TempAnimMontage = MonsterData->BOW_UPPER_START;
		break;
	case EMonsterMontage::BOW_UPPER_END:
		TempAnimMontage = MonsterData->BOW_UPPER_END;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_END;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_START:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_START;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_END:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_END;
		break;
	case EMonsterMontage::ATTACK_FIRE_START:
		TempAnimMontage = MonsterData->ATTACK_FIRE_START;
		break;
	case EMonsterMontage::ATTACK_FIRE:
		TempAnimMontage = MonsterData->ATTACK_FIRE;
		break;
	case EMonsterMontage::ATTACK_FIRE_END:
		TempAnimMontage = MonsterData->ATTACK_FIRE_END;
		break;
	case EMonsterMontage::ATTACK_HORN_START:
		TempAnimMontage = MonsterData->ATTACK_HORN_START;
		break;
	case EMonsterMontage::ATTACK_HORN_END:
		TempAnimMontage = MonsterData->ATTACK_HORN_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_END;
		break;
	case EMonsterMontage::REBOUND:
		TempAnimMontage = MonsterData->REBOUND;
		break;
	case EMonsterMontage::RODEO_START:
		TempAnimMontage = MonsterData->RODEO_START;
		break;
	case EMonsterMontage::RODEO_END:
		TempAnimMontage = MonsterData->RODEO_END;
		break;
	case EMonsterMontage::STUN_START:
		TempAnimMontage = MonsterData->STUN_START;
		break;
	case EMonsterMontage::STUN_END:
		TempAnimMontage = MonsterData->STUN_END;
		break;
	case EMonsterMontage::TURN_180_L:
		TempAnimMontage = MonsterData->TURN_180_L;
		break;
	case EMonsterMontage::TURN_180_R:
		TempAnimMontage = MonsterData->TURN_180_R;
		break;
	case EMonsterMontage::DRAW_BOW:
		TempAnimMontage = MonsterData->DRAW_BOW;
		break;
	case EMonsterMontage::SHEATH_BOW:
		TempAnimMontage = MonsterData->SHEATH_BOW;
		break;
	case EMonsterMontage::BOW_TO_SWORD:
		TempAnimMontage = MonsterData->BOW_TO_SWORD;
		break;
	case EMonsterMontage::SWORD_TO_BOW:
		TempAnimMontage = MonsterData->SWORD_TO_BOW;
		break;
	case EMonsterMontage::DRAW_LSWORD:
		TempAnimMontage = MonsterData->DRAW_LSWORD;
		break;
	case EMonsterMontage::SHEATH_LSWORD:
		TempAnimMontage = MonsterData->SHEATH_LSWORD;
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