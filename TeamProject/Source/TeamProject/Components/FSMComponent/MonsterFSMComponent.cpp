// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/MonsterFSMComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "Actors/Monster/Monster.h"
#include "Actors/Object/PatrolPath.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"

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
	case EMonsterState::Combat:
		UpdateCombat(DeltaTime);
		break;
	default:
		break;
	}
}

void UMonsterFSMComponent::ChangeState(EMonsterState NewState)
{
	if (eCurrentState == NewState) { return; }
	switch (NewState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Patrol:
		break;
	case EMonsterState::Suspicious:
		break;
	case EMonsterState::Alert:
		// Play Alert Montage;
		break;
	case EMonsterState::Combat:
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

	if (eCurrentState == EMonsterState::Idle)
	{

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
	MoveToLocation(Location);


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
}

void UMonsterFSMComponent::UpdateSuspicious(float DeltaTime)
{
	if (SuspicionGauge >= MaxSuspicionGauge)
	{
		ChangeState(EMonsterState::Alert);
	}
	else
	{
		SuspicionGauge += DeltaTime;
	}
}

void UMonsterFSMComponent::UpdateAlert(float DeltaTime)
{
	// Actually do nothing in here
}

void UMonsterFSMComponent::UpdateCombat(float DeltaTime)
{
	// Chase and Attack Player;
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
