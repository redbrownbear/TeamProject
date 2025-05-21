// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "Actors/Monster/Monster.h"
#include "Actors/Object/PatrolPath.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Object/CampFire.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Item/WorldWeapon.h"

#include "Navigation/PathFollowingComponent.h"


UMonsterFSMComponent::UMonsterFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMonsterFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	switch (eGroupType)
	{
	case EMonsterGroupType::Patrol:
		ChangeState(EMonsterState::Patrol);
		break;
	case EMonsterGroupType::TreasureBox:
		ChangeState(EMonsterState::Idle);
		break;
	case EMonsterGroupType::Alone:
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::BeginPlay // No GroupType"));
		check(false);
		break;
	case EMonsterGroupType::End:
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::BeginPlay // No GroupType"));
		check(false);
		break;
	default:
		break;
	}
}


// Called every frame
void UMonsterFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UMonsterFSMComponent::SetMonsterGroupType(EMonsterGroupType NewGroupType)
{
	eGroupType = NewGroupType;

	switch (eGroupType)
	{
	case EMonsterGroupType::Patrol:
		if (EMonsterState::Idle == eCurrentState)
		{
			ChangeState(EMonsterState::Patrol);
		}
		break;
	case EMonsterGroupType::TreasureBox:
		if (EMonsterState::Patrol == eCurrentState)
		{
			ChangeState(EMonsterState::Idle);
		}
		break;
	case EMonsterGroupType::Alone:
		break;
	case EMonsterGroupType::End:
		break;
	default:
		break;
	}
}

void UMonsterFSMComponent::HandleState(float DeltaTime)
{
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::HandleState // No Owner"));
		check(false);
		return;
	}

	switch (eCurrentState)
	{
	case EMonsterState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case EMonsterState::Patrol:
		UpdatePatrol(DeltaTime);
		break;
	case EMonsterState::Suspicious:
		UpdateSuspicious(DeltaTime);
		break;
	case EMonsterState::Alert:
		UpdateAlert(DeltaTime);
		break;
	case EMonsterState::FindWeapon:
		UpdateFindWeapon(DeltaTime);
		break;
	case EMonsterState::Combat:
		UpdateCombat(DeltaTime);
		break;
	case EMonsterState::Dance:
		UpdateDance(DeltaTime);
		break;
	case EMonsterState::ToDance:
		UpdateToDance(DeltaTime);
		break;
	case EMonsterState::Signal:
		UpdateSignal(DeltaTime);
		break;
	default:
		break;
	}
}

void UMonsterFSMComponent::ChangeState(EMonsterState NewState)
{
	if (eCurrentState == NewState) { return; }

	const EMonsterState PrevState = eCurrentState;
	switch (PrevState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Patrol:
		break;
	case EMonsterState::Suspicious:
		SuspicionGauge = 0.f;
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:
	{
		Owner->SetSpeedWalk();
	}
		break;
	case EMonsterState::FindWeapon:
		break;

	case EMonsterState::Dead:
		break;
	case EMonsterState::Fire:
		break;
	case EMonsterState::Eat:
		break;
	case EMonsterState::Dance:
		break;
	case EMonsterState::Signal:
		Owner->PlayMontage(EMonsterMontage::SIGNAL_END);
		break;
	case EMonsterState::End:
		break;
	default:
		break;
	}


	switch (NewState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Patrol:
		break;
	case EMonsterState::Suspicious:
		break;
	case EMonsterState::Alert:
		Owner->PlayMontage(EMonsterMontage::FIND);
		break;
	case EMonsterState::FindWeapon:
		if (CatchedWeapon)
		{
			return;
		}


		if (PrevState != EMonsterState::Combat)
		{
			Owner->PlayMontage(EMonsterMontage::ANGRY);
		}
		Owner->SetSpeedRun();
		break;
	case EMonsterState::Combat:
		break;
	case EMonsterState::Dance:
		Owner->PlayMontage(EMonsterMontage::DANCE_START);
		break;
	case EMonsterState::Signal:
		{
			Owner->PlayMontage(EMonsterMontage::SIGNAL_START);
			SpawnProjectile(ProjectileName::Monster_PlayerAlert, CollisionProfileName::ToMonster);
		}
		break;
	default:
		break;
	}
	eCurrentState = NewState;
}

void UMonsterFSMComponent::UpdateIdle(float DeltaTime)
{
	// if it is PatrolGroup, it's error
	if (eGroupType == EMonsterGroupType::Patrol)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateIdle // This Monster is PatrolGroup, not TreasureBoxGroup"));
		check(false);
	}


	ChangeState(EMonsterState::ToDance);
}

void UMonsterFSMComponent::UpdateDance(float DeltaTime)
{
	if (AActor* CampFireActor = Owner->GetCampFire())
	{
		const FVector CampFireLocation = CampFireActor->GetActorLocation();
		SmoothRotateActorToDirection(Owner, CampFireLocation, DeltaTime);
	}


	if (Player)
	{
		ChangeState(EMonsterState::Suspicious);
	}
}

void UMonsterFSMComponent::UpdateToDance(float DeltaTime)
{
	if (AActor* CampFireActor = Owner->GetCampFire())
	{
		const FVector CampFireLocation = CampFireActor->GetActorLocation();

		MoveToLocation(CampFireLocation);

		float fDistance = FVector::Dist(Owner->GetActorLocation(), CampFireLocation);
		if (fDistance < MONSTER_CAMPFIRE_MIN_LENGTH)
		{
			StopMove();
			ChangeState(EMonsterState::Dance);
		}
	}

	if (Player)
	{
		ChangeState(EMonsterState::Suspicious);
	}
}

void UMonsterFSMComponent::UpdatePatrol(float DeltaTime)
{
	// if it is TreasureBoxGroup, it's error
	if (eGroupType == EMonsterGroupType::TreasureBox)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // This Monster is TreasureBoxGroup, not PatrolGroup"));
		check(false);
	}


	// 목표 위치 구하기
	FVector Location = FVector();

	if (APatrolPath* PatrolPath = Owner->GetPatrolPath())
	{
		Location = PatrolPath->GetSplinePointLocation(CurrentPatrolIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // No PatrolPath"));
		//check(false);
		return;
	}

	// 이동
	if (Owner->IsPlayingMontage(EMonsterMontage::END))
	{
		StopMove();
	}
	else
	{
		MoveToLocation(Location);
	}

	// 다음 PatrolIndex 구하기
	const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), Location, 150.f);

	if (bIsNear)
	{
		++CurrentPatrolIndex;

		if (APatrolPath* PatrolPath = Owner->GetPatrolPath())
		{
			if (CurrentPatrolIndex >= PatrolPath->GetSplineMaxIndex())
			{
				CurrentPatrolIndex = 0;
			}
		}
	}

	if (Player)
	{
		StopMove();
		ChangeState(EMonsterState::Suspicious);
	}
}

void UMonsterFSMComponent::UpdateSuspicious(float DeltaTime)
{
	if (Player)
	{
		SuspicionGauge += DeltaTime * MONSTER_SUSPICIOUS_COEFFICIENT;

		const FVector MonsterLocation = Owner->GetActorLocation();
		const FVector PlayerLocation = Player->GetActorLocation();
		const float fDistance = FVector::Dist(MonsterLocation, PlayerLocation);

		if (SuspicionGauge >= MaxSuspicionGauge
			|| fDistance < MONSTER_IMMEDIATE_ALERT_RADIUS
			)
		{
			SuspicionGauge = 0.f;
			ChangeState(EMonsterState::Alert);
		}
		else
		{
			SuspicionGauge += DeltaTime;
		}

		SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);

	}
	else
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UMonsterFSMComponent::UpdateAlert(float DeltaTime)
{
	// Actually do nothing in here
}

void UMonsterFSMComponent::UpdateFindWeapon(float DeltaTime)
{
	if (ToCatchWeapon)
	{
		if (ToCatchWeapon->GetCatched())
		{
			ChangeState(EMonsterState::Combat);
		}

		if (CatchedWeapon)
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateFindWeapon // ToCatchWeapon, CatchedWeapon can't exist together "));
			check(false);
			return;
		}

		// 이동
		const FVector Location = ToCatchWeapon->GetActorLocation();
		if (Owner->IsPlayingMontage(EMonsterMontage::END))
		{
			StopMove();
		}
		else
		{
			MoveToLocation(Location);
		}

		// 다음 PatrolIndex 구하기
		const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), Location, 100.f);

		if (bIsNear)
		{
			StopMove();
			Owner->PlayMontage(EMonsterMontage::WEAPON_CATCH);
		}
	}
	else
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UMonsterFSMComponent::UpdateCombat(float DeltaTime)
{
	if (!IsValid(Player)) 
	{
		ChangeState(EMonsterState::Idle); 
		return;
	}

	// Get Target Location
	FVector Location = Player->GetActorLocation();

	// Move
	// 이동
	if (Owner->IsPlayingMontage(EMonsterMontage::END))
	{
		StopMove();
	}
	else
	{
		MoveToLocation(Location);
	}

	// 공격 쿨타임 갱신
	CurrentAttackCoolTime += DeltaTime;



	// Check if it's arrived
	const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), Location, 150.f);

	if (bIsNear)
	{
		StopMove();
		if (CurrentAttackCoolTime > MONSTER_ATTACK_COOLTIME)
		{
			CurrentAttackCoolTime = 0.f;
			Owner->PlayMontage(EMonsterMontage::ATTACK);
		}
	}
}

void UMonsterFSMComponent::UpdateSignal(float DeltaTime)
{
	SignalElapsedTime += DeltaTime;
	if (SignalElapsedTime > MONSTER_MAX_SIGNAL_TIME)
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UMonsterFSMComponent::MoveToLocation(const FVector& InLocation)
{

	if (AAIController* AIController = Cast<AAIController>(Owner->GetController()))
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(InLocation);
		MoveRequest.SetAcceptanceRadius(50.f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::MoveToLocation // No AIController"));
		check(false);
	}
}

void UMonsterFSMComponent::StopMove()
{
	if (AAIController* AIController = Cast<AAIController>(Owner->GetController()))
	{
		AIController->StopMovement();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::StopMove // No AIController"));
		check(false);
	}
}

void UMonsterFSMComponent::SpawnProjectile(FName ProjectileName, FName CollisionProfileName)
{
	UWorld* World = GetWorld();

	AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
		FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	FTransform NewTransform;
	Projectile->SetData(ProjectileName, CollisionProfileName);
	const FVector Location = Owner->GetActorLocation();

	NewTransform.SetLocation(Location);
	NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
	Projectile->FinishSpawning(NewTransform);
}
