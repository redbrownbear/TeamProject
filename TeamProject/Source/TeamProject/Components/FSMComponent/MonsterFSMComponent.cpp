// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/MonsterFSMComponent.h"

UMonsterFSMComponent::UMonsterFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMonsterFSMComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UMonsterFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UMonsterFSMComponent::HandleState(float DeltaTime)
{
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
	}
}

void UMonsterFSMComponent::UpdatePatrol(float DeltaTime)
{
	// if it is TreasureBoxGroup, it's error
	if (eGroupType == EMonsterGroupType::TreasureBox)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // This Monster is TreasureBoxGroup, not PatrolGroup"));
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
