// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void ULynelFSMComponent::HandleState(float DeltaTime)
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
	case EMonsterState::AimingBow:
		UpdateAimingBow(DeltaTime);
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

	//
	
	
	
	
	
	
	
	
	
}

void ULynelFSMComponent::ChangeState(EMonsterState NewState)
{
}

void ULynelFSMComponent::UpdateIdle(float DeltaTime)
{
}

void ULynelFSMComponent::UpdatePatrol(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateSuspicious(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateAlert(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateCombat(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateAimingBow(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateAimingBowUpper(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateDashAttack(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateExplosionAttack(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateFireAttack(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateHornAttack(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateRunningAttack(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateDead(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateRebound(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateRodeo(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateStun(float DeltaTime)
{
}
