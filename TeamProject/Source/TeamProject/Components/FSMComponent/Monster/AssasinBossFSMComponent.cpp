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


		ePhase = EAssasinBossPhase::PHASE_2;

		//if (HPRate > 0.66f)
		//{
		//	ePhase = EAssasinBossPhase::PHASE_1;
		//}
		//else if (HPRate > 0.33f)
		//{
		//	ePhase = EAssasinBossPhase::PHASE_2;
		//}
		//else
		//{
		//	ePhase = EAssasinBossPhase::PHASE_3;
		//}


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
		CharacterMonster->PlayMontage(EMonsterMontage::BARRIER_START);
		break;
	case EMonsterState::Stun:
		EnableFlyingMode(false);
		CharacterMonster->PlayMontage(EMonsterMontage::STUN_START);
		break;
	case EMonsterState::Dead:
		EnableFlyingMode(false);
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
	// 캐릭터의 하단부에서 트레이스를 시작하여 정확도를 높일 수 있습니다.
	// TraceStart = CurrentLocation - FVector(0,0,CharacterMonster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	const FVector TraceEnd = TraceStart - FVector(0, 0, 1) * 2000.0f; // 충분히 긴 트레이스 거리

	// 지면 감지 (라인 트레이스)
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility, // 또는 지형에 맞는 콜리전 채널 (예: ECC_WorldStatic)
		FCollisionQueryParams(TEXT("HoverTrace"), true, CharacterMonster) // 자신을 무시하도록 CharacterMonster 추가
	);

	float TargetZ = CurrentLocation.Z; // 기본적으로 현재 Z 유지
	bool bShouldAdjustZ = false;

	if (bHit)
	{
		// 목표 Z 위치: 지면 + 원하는 부유 높이
		// KOGASTONE_ORBIT_RADIUS * 1.5f 가 부유 높이로 의도된 값이라면 이대로 사용
		const float DesiredHoverHeight = KOGASTONE_ORBIT_RADIUS * 1.5f;
		TargetZ = HitResult.Location.Z + DesiredHoverHeight;
		bShouldAdjustZ = true; // 지면을 찾았으므로 Z축 조절 필요
	}
	// else: 지면을 찾지 못했다면, 이 경우 자유 낙하 (GravityScale=0이므로 그대로 유지)하거나
	// 특정 동작을 정의해야 합니다. 현재는 Z를 조절하지 않고 현재 Z값을 유지합니다.

	if (bShouldAdjustZ)
	{
		// 현재 Z 위치와 목표 Z 위치 간의 차이
		float ZDifference = TargetZ - CurrentLocation.Z;

		// 목표 Z 위치로 부드럽게 이동하기 위한 Z축 속도 조절
		// P(비례) 제어기처럼 작동하여 목표에 가까워질수록 속도가 줄어듭니다.
		// 여기에 원하는 '복원력' 혹은 '부유력' 상수를 곱하여 속도를 제어합니다.
		const float HoverRecoverySpeed = 500.0f; // 이 값을 조절하여 부유 높이로 돌아가는 속도 제어 (cm/s)
		float ZVelocity = ZDifference * HoverRecoverySpeed;

		// 너무 급격한 속도 변화를 막기 위해 최대 Z 속도를 제한할 수 있습니다.
		ZVelocity = FMath::Clamp(ZVelocity, -MovementComponent->MaxFlySpeed, MovementComponent->MaxFlySpeed);

		// 현재 캐릭터의 속도를 가져와 Z축만 변경
		FVector CurrentVelocity = MovementComponent->Velocity;
		CurrentVelocity.Z = ZVelocity;

		MovementComponent->Velocity = CurrentVelocity; // CharacterMovementComponent의 Velocity 직접 설정

		// MoveUpdatedComponent 대신 CharacterMovementComponent가 Velocity를 기반으로 이동하도록 합니다.
		// PhysCustom에서 직접 MoveUpdatedComponent를 호출할 때는 Velocity를 이용해야 합니다.
		// 하지만 여기서는 UpdateCombat에서 Hovering을 호출하고, CharacterMovementComponent가 자체적으로 틱에서 Velocity를 처리하므로
		// MovementComponent->Velocity = CurrentVelocity; 만으로도 충분합니다.
	}
	else
	{
		// 지면을 찾지 못했다면 Z축 속도를 0으로 설정하거나, 다른 로직을 적용
		FVector CurrentVelocity = MovementComponent->Velocity;
		CurrentVelocity.Z = 0.0f; // 더 이상 Z축으로 이동하지 않음
		MovementComponent->Velocity = CurrentVelocity;
	}

	// Horizontal Movement (수평 이동)
	// UpdateCombat에서 이미 SmoothRotateActorToDirection을 호출하고 있으므로,
	// AddMovementInput은 다른 곳에서 처리되고 있을 것입니다.
	// CharacterMovementComponent는 자체적으로 Velocity를 기반으로 이동을 처리합니다.
}

