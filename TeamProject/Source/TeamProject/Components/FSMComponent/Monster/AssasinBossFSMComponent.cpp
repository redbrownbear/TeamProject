// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "GameFramework/CharacterMovementComponent.h"


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

	if (UMonsterStatusComponent* StatusComponent = CharacterMonster->GetStatusComponent())
	{
		const float CurrentHP = StatusComponent->GetCurrentHP();
		const float MaxHP = StatusComponent->GetMaxHP();

		const float HPRate = CurrentHP / MaxHP;


		//ePhase = EAssasinBossPhase::PHASE_3;

		if (HPRate > 0.66f)
		{
			ePhase = EAssasinBossPhase::PHASE_1;
		}
		else if (HPRate > 0.33f)
		{
			ePhase = EAssasinBossPhase::PHASE_2;
		}
		else
		{
			ePhase = EAssasinBossPhase::PHASE_3;
		}


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
	case EMonsterState::Damage:
		UpdateDamage(DeltaTime);
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
		EnableFlyingMode(false);
		break;
	case EMonsterState::Combat:
		EnableFlyingMode(true);
		CharacterMonster->PlayMontage(EMonsterMontage::APPEAR_START);
		break;
	case EMonsterState::Stone:
		switch (ePhase)
		{
		case EAssasinBossPhase::PHASE_1:
		case EAssasinBossPhase::PHASE_3:
			UE_LOG(LogTemp, Warning, TEXT("EAssasinBossPhase::PHASE_1, 3"));
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BALL_FIRST_START);
			break;
		case EAssasinBossPhase::PHASE_2:
			UE_LOG(LogTemp, Warning, TEXT("EAssasinBossPhase::PHASE_2"));
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_BALL_SECOND_START);
			break;
		case EAssasinBossPhase::END:
			break;
		default:
			break;
		}
		break;
	case EMonsterState::Barrier:
		BarrierElapsedTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::BARRIER_START);
		break;
	case EMonsterState::Stun:
		StunElapsedTime = 0.f;
		EnableFlyingMode(false);
		CharacterMonster->PlayMontage(EMonsterMontage::STUN_START);
		break;
	case EMonsterState::Dead:
		EnableFlyingMode(false);
		CharacterMonster->PlayMontage(EMonsterMontage::DEAD);
		break;
	case EMonsterState::Damage:
		DamageElapsedTime = 0.f;
		CharacterMonster->StopAnimMontage();
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

	Hovering(DeltaTime);

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
	if (StunElapsedTime > ASSASIN_BOSS_STUN_MAX_TIME)
	{
		StunElapsedTime = 0.f;
		ChangeState(EMonsterState::Combat);
	}
}

void UAssasinBossFSMComponent::UpdateBarrier(float DeltaTime)
{
	StopMove();

	Hovering(DeltaTime);

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

	Hovering(DeltaTime);


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

void UAssasinBossFSMComponent::UpdateDamage(float DeltaTime)
{
	StopMove();

	DamageElapsedTime += DeltaTime;
	if (DamageElapsedTime > ASSASIN_BOSS_DAMAGE_MAX_TIME)
	{
		DamageElapsedTime = 0.f;
		ChangeState(EMonsterState::Combat);
	}
}

void UAssasinBossFSMComponent::EnableFlyingMode(bool bFlag)
{
	if (UCharacterMovementComponent * MovementComponent = CharacterMonster->GetCharacterMovement())
	{
		if (bFlag)
		{
			MovementComponent->GravityScale = 0.0f;
			MovementComponent->SetMovementMode(MOVE_Flying);
			MovementComponent->MaxFlySpeed = MovementComponent->MaxWalkSpeed;
		}
		else
		{
			MovementComponent->GravityScale = 1.0f; // 또는 기본값으로
			MovementComponent->SetMovementMode(MOVE_Walking);
		}
	}
}

void UAssasinBossFSMComponent::Hovering(float DeltaTime)
{
	if (!CharacterMonster || !CharacterMonster->GetCharacterMovement())
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = CharacterMonster->GetCharacterMovement();
	const FVector CurrentLocation = CharacterMonster->GetActorLocation();
	FHitResult HitResult;
	const FVector TraceStart = CurrentLocation;
	const FVector TraceEnd = TraceStart - FVector(0, 0, 1) * 2000.0f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility, 
		FCollisionQueryParams(TEXT("HoverTrace"), true, CharacterMonster) 
	);

	float TargetZ = CurrentLocation.Z;
	bool bShouldAdjustZ = false;

	if (bHit)
	{
		const float DesiredHoverHeight = KOGASTONE_ORBIT_RADIUS * 1.5f;
		TargetZ = HitResult.Location.Z + DesiredHoverHeight;
		bShouldAdjustZ = true; 
	}

	if (bShouldAdjustZ)
	{
		float ZDifference = TargetZ - CurrentLocation.Z;

		const float HoverRecoverySpeed = 500.0f;
		float ZVelocity = ZDifference * HoverRecoverySpeed;

		ZVelocity = FMath::Clamp(ZVelocity, -MovementComponent->MaxFlySpeed, MovementComponent->MaxFlySpeed);

		FVector CurrentVelocity = MovementComponent->Velocity;
		CurrentVelocity.Z = ZVelocity;

		MovementComponent->Velocity = CurrentVelocity; 
	}
	else
	{
		FVector CurrentVelocity = MovementComponent->Velocity;
		CurrentVelocity.Z = 0.0f;
		MovementComponent->Velocity = CurrentVelocity;
	}
}

