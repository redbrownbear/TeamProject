// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/AssasinLeaderFSMComponent.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Object/PatrolPath.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Misc/Utils.h"

UAssasinLeaderFSMComponent::UAssasinLeaderFSMComponent()
{
	eCurrentState = EMonsterState::Patrol;
}

void UAssasinLeaderFSMComponent::HandleState(float DeltaTime)
{
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UAssasinLeaderFSMComponent::HandleState // No CharacterMonster"));
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
	case EMonsterState::Signal:
		UpdateSignal(DeltaTime);
		break;
	case EMonsterState::Combat:
		UpdateCombat(DeltaTime);
		break;
	case EMonsterState::Happy:
		UpdateHappy(DeltaTime);
		break;
	case EMonsterState::Dead:
		UpdateDying(DeltaTime);
		break;
	case EMonsterState::End:
	default:
		UE_LOG(LogTemp, Error, TEXT("UAssasinLeaderFSMComponent::HandleState // Unexpected MonsterState"));
		check(false);
		break;
	}
}

void UAssasinLeaderFSMComponent::ChangeState(EMonsterState NewState)
{
	if (eCurrentState == NewState) { return; }
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UAssasinLeaderFSMComponent::ChangeState // No CharacterMonster"));
		return;
	}


	const EMonsterState PrevState = eCurrentState;

	switch (PrevState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Patrol:
		break;
	case EMonsterState::Suspicious:
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Signal:
		break;
	case EMonsterState::Combat:
		CharacterMonster->PlayMontage(EMonsterMontage::SEARCH);
		break;
	case EMonsterState::Happy:
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
		CharacterMonster->PlayMontage(EMonsterMontage::FIND);
		break;
	case EMonsterState::Signal:
		CharacterMonster->PlayMontage(EMonsterMontage::SIGNAL_START);
		break;
	case EMonsterState::Combat:
		CharacterMonster->PlayMontage(EMonsterMontage::DRAW_LSWORD);
		break;
	case EMonsterState::Happy:
		break;
	}

	eCurrentState = NewState;
}

void UAssasinLeaderFSMComponent::UpdateIdle(float DeltaTime)
{
	ChangeState(EMonsterState::Patrol);
}

void UAssasinLeaderFSMComponent::UpdatePatrol(float DeltaTime)
{
	if (CharacterMonster->IsPlayingMontage())
	{
		return;
	}


	// 목표 위치 구하기
	FVector Location = FVector();

	if (PatrolPath)
	{
		Location = PatrolPath->GetSplinePointLocation(CurrentPatrolIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UAssasinLeaderFSMComponent::UpdatePatrol // No PatrolPath"));
		check(false);
		return;
	}

	MoveToLocation(Location);

	// 다음 PatrolIndex 구하기
	const bool bIsNear = FVector::PointsAreNear(CharacterMonster->GetActorLocation(), Location, MONSTER_DEFAULT_NEAR_DISTANCE);

	if (bIsNear)
	{
		CharacterMonster->PlayMontage(EMonsterMontage::SEARCH);

		++CurrentPatrolIndex;


		if (PatrolPath)
		{
			if (CurrentPatrolIndex >= PatrolPath->GetSplineMaxIndex())
			{
				CurrentPatrolIndex = 0;
			}
		}
	}

	if (Player)
	{
		this->StopMove();
		ChangeState(EMonsterState::Suspicious);
	}
}

void UAssasinLeaderFSMComponent::UpdateSuspicious(float DeltaTime)
{
	Super::UpdateSuspicious(DeltaTime);
}

void UAssasinLeaderFSMComponent::UpdateAlert(float DeltaTime)
{
	if (!CharacterMonster->IsPlayingMontage())
	{
		ChangeState(EMonsterState::Signal);
	}
}

void UAssasinLeaderFSMComponent::UpdateSignal(float DeltaTime)
{
	if (!CharacterMonster->IsPlayingMontage())
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UAssasinLeaderFSMComponent::UpdateCombat(float DeltaTime)
{
	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = CharacterMonster->GetActorLocation();

	SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime, 10.f);

	AttackCoolTime += DeltaTime;
	AttackBigCoolTime += DeltaTime;

	if (AttackBigCoolTime > ASSASIN_LEADER_ATTACK_BIG_COOLTIME_MAX)
	{
		AttackBigCoolTime = 0.f;
		AttackCoolTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BIG);
	}



	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(CharacterMonster->GetMovementComponent());
	if (AttackCoolTime > ASSASIN_LEADER_ATTACK_COOLTIME_MAX)
	{
		if (!CharacterMonster->IsPlayingMontage())
		{
			MoveToLocation(PlayerLocation);
		}

		const bool bIsNear = FVector::PointsAreNear(PlayerLocation, MonsterLocation, MONSTER_DEFAULT_NEAR_DISTANCE);
		if (bIsNear)
		{
			AttackCoolTime = 0.f;
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK);
		}
		MovementComponent->bOrientRotationToMovement = false;
	}
	else
	{
		const float fDistance = FVector::Dist(PlayerLocation, MonsterLocation);
	}
}




void UAssasinLeaderFSMComponent::UpdateHappy(float DeltaTime)
{
}
