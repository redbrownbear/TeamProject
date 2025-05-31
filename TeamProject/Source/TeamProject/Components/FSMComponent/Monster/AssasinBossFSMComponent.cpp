// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Character/PlayerCharacter.h"


UAssasinBossFSMComponent::UAssasinBossFSMComponent()
{
	eCurrentState = EMonsterState::Idle;
}

void UAssasinBossFSMComponent::HandleState(float DeltaTime)
{
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UAssasinBossFSMComponent::HandleState // No CharacterMonster"));
		check(false);
		return;
	}

	switch (eCurrentState)
	{
	case EMonsterState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case EMonsterState::Combat:
		UpdateCombat(DeltaTime);
		break;
	case EMonsterState::Dead:
		UpdateDying(DeltaTime);
		break;
	case EMonsterState::Stun:
		UpdateStun(DeltaTime);
		break;
	case EMonsterState::Stone:
		UpdateStone(DeltaTime);
		break;
	case EMonsterState::Barrier:
		UpdateBarrier(DeltaTime);
		break;
	case EMonsterState::End:
	default:
		UE_LOG(LogTemp, Error, TEXT("UAssasinBossFSMComponent::HandleState // Unexpected MonsterState"));
		check(false);
		break;
	}
}

void UAssasinBossFSMComponent::ChangeState(EMonsterState NewState)
{
	if (eCurrentState == NewState) { return; }
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UAssasinBossFSMComponent::ChangeState // No CharacterMonster"));
		return;
	}


	const EMonsterState PrevState = eCurrentState;

	switch (PrevState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Combat:
		break;
	case EMonsterState::Barrier:
		CharacterMonster->PlayMontage(EMonsterMontage::BARRIER_END);
		eCombat = EAssasinBossCombat::STONE;
		break;
	case EMonsterState::Stone:
		switch (ePhase)
		{
		case EAssasinBossPhase::PHASE_1:
		case EAssasinBossPhase::PHASE_3:
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BALL_FIRST_END);
			break;
		case EAssasinBossPhase::PHASE_2:
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BALL_SECOND_END);
			break;
		case EAssasinBossPhase::END:
			break;
		default:
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("ChangeState // PrevState Stone"))
		eCombat = EAssasinBossCombat::BARRIER;
		break;
	case EMonsterState::Stun:
		CharacterMonster->PlayMontage(EMonsterMontage::STUN_END);
		break;
	default:
		break;
	}

	switch (NewState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Combat:
		CharacterMonster->PlayMontage(EMonsterMontage::APPEAR_START);
		break;
	case EMonsterState::Stone:
		switch (ePhase)
		{
		case EAssasinBossPhase::PHASE_1:
		case EAssasinBossPhase::PHASE_3:
			UE_LOG(LogTemp, Warning, TEXT("EAssasinBossPhase::PHASE_1, 3"));
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BALL_FIRST_END);
			break;
		case EAssasinBossPhase::PHASE_2:
			UE_LOG(LogTemp, Warning, TEXT("EAssasinBossPhase::PHASE_2"));
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BALL_SECOND_END);
			break;
		case EAssasinBossPhase::END:
			break;
		default:
			break;
		}
		break;
	case EMonsterState::Barrier:
		CharacterMonster->PlayMontage(EMonsterMontage::BARRIER_START);
		break;
	case EMonsterState::Stun:
		CharacterMonster->PlayMontage(EMonsterMontage::STUN_START);
		break;
	case EMonsterState::Dead:
		CharacterMonster->PlayMontage(EMonsterMontage::DEAD);
		break;
	}

	eCurrentState = NewState;
}

void UAssasinBossFSMComponent::UpdateIdle(float DeltaTime)
{
	StopMove();

	if (Player)
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UAssasinBossFSMComponent::UpdateCombat(float DeltaTime)
{
	if (!Player)
	{
		ToNextElapsedTime = 0.f;
		ChangeState(EMonsterState::Idle);
	}


	ToNextElapsedTime += DeltaTime;


	const FVector PlayerLocation = Player->GetActorLocation();
	SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);

	if (ToNextElapsedTime > ASSASIN_BOSS_TONEXT_MAX_TIME)
	{
		ToNextElapsedTime = 0.f;

		switch (eCombat)
		{
		case EAssasinBossCombat::BARRIER:
			ChangeState(EMonsterState::Barrier);
			break;
		case EAssasinBossCombat::STONE:
			ChangeState(EMonsterState::Stone);
			break;
		case EAssasinBossCombat::END:
			break;
		default:
			break;
		}
	}

}

void UAssasinBossFSMComponent::UpdateStun(float DeltaTime)
{
	StopMove();
	StunElapsedTime += DeltaTime;
	if (StoneElapsedTime > ASSASIN_BOSS_STUN_MAX_TIME)
	{
		StoneElapsedTime = 0.f;
		ChangeState(EMonsterState::Combat);
	}
}

void UAssasinBossFSMComponent::UpdateBarrier(float DeltaTime)
{
	StopMove();
	BarrierElapsedTime += DeltaTime;
	if (BarrierElapsedTime > ASSASIN_BOSS_BARRIER_MAX_TIME)
	{
		BarrierElapsedTime = 0.f;
		ChangeState(EMonsterState::Combat);
	}
}

void UAssasinBossFSMComponent::UpdateStone(float DeltaTime)
{
	StopMove();

	if (!Player)
	{
		StoneElapsedTime = 0.f;
		ChangeState(EMonsterState::Idle);
	}

	const FVector PlayerLocation = Player->GetActorLocation();

	SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);

	StoneElapsedTime += DeltaTime;

	switch (ePhase)
	{
	case EAssasinBossPhase::PHASE_1:
		if (StoneElapsedTime > ASSASIN_BOSS_STONE_FIRST_MAX_TIME)
		{
			StoneElapsedTime = 0.f;
			ChangeState(EMonsterState::Combat);
		}
		break;
	case EAssasinBossPhase::PHASE_2:
		if (StoneElapsedTime > ASSASIN_BOSS_STONE_SECOND_MAX_TIME)
		{
			StoneElapsedTime = 0.f;
			ChangeState(EMonsterState::Combat);
		}
		break;
	case EAssasinBossPhase::PHASE_3:
		if (StoneElapsedTime > ASSASIN_BOSS_STONE_FIRST_MAX_TIME)
		{
			StoneElapsedTime = 0.f;
			ChangeState(EMonsterState::Combat);
		}
		break;
	case EAssasinBossPhase::END:
		break;
	default:
		break;
	}

}

