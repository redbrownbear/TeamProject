// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

#include "Actors/Monster/MonsterInterface.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"

#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "GameFramework/PC_InGame.h"

#include "UI/HUD/MainHUD.h"

#include "Data/MonsterTableRow.h"

ULynelFSMComponent::ULynelFSMComponent()
{
	eCurrentState = EMonsterState::Idle;
}

void ULynelFSMComponent::HandleState(float DeltaTime)
{
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::HandleState // No CharacterMonster"));
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
	case EMonsterState::Dead:
		UpdateDying(DeltaTime);
		break;
	case EMonsterState::AimingBowUpper:
		UpdateAimingBowUpper(DeltaTime);
		break;
	case EMonsterState::DashAttack:
		UpdateDashAttack(DeltaTime);
		break;
	case EMonsterState::ExplosionAttack:
		UpdateExplosionAttack(DeltaTime);
		break;
	case EMonsterState::FireAttack:
		UpdateFireAttack(DeltaTime);
		break;
	case EMonsterState::HornAttack:
		UpdateHornAttack(DeltaTime);
		break;
	case EMonsterState::RunningAttack:
		UpdateRunningAttack(DeltaTime);
		break;
	case EMonsterState::Rebound:
		UpdateRebound(DeltaTime);
		break;
	case EMonsterState::Rodeo:
		UpdateRodeo(DeltaTime);
		break;
	case EMonsterState::Stun:
		UpdateStun(DeltaTime);
		break;
	case EMonsterState::ReadyToAttack:
		UpdateReadyToAttack(DeltaTime);
		break;
	case EMonsterState::Temp:
		UpdateTemp(DeltaTime);
		break;
	case EMonsterState::End:
	default:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::HandleState // Unexpected MonsterState"));
		check(false);
		break;
	}
}

void ULynelFSMComponent::ChangeState(EMonsterState NewState)
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
		break;
	case EMonsterState::FindWeapon:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Dead:
		break;
	case EMonsterState::Fire:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Eat:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Dance:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Signal:
		break;
	case EMonsterState::AimingBow:
		break;
	case EMonsterState::AimingBowUpper:
		break;
	case EMonsterState::DashAttack:
		eReadyToAttackStep = EReadyToAttackStep::RunToLink;
		break;
	case EMonsterState::ExplosionAttack:
		break;
	case EMonsterState::FireAttack:
		break;
	case EMonsterState::HornAttack:
		eReadyToAttackStep = EReadyToAttackStep::RunToLink;
		break;
	case EMonsterState::RunningAttack:
		eReadyToAttackStep = EReadyToAttackStep::RunToLink;
		break;
	case EMonsterState::Rebound:
		break;
	case EMonsterState::Rodeo:
		break;
	case EMonsterState::Stun:
		break;
	case EMonsterState::ReadyToAttack:
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
	{
		float ReturnValue = GetSideOfActor(CharacterMonster, Player);
		// RightSide
		if (ReturnValue > 0.f)
			CharacterMonster->PlayMontage(EMonsterMontage::TURN_180_R);
		else
			CharacterMonster->PlayMontage(EMonsterMontage::TURN_180_L);
	}
		break;
	case EMonsterState::Alert:
		CharacterMonster->PlayMontage(EMonsterMontage::SIGNAL_START);
		// @TODO : Effect
		break;
	case EMonsterState::Combat:
		// To show Combat Pattern in order
		switch (eCombatIndex)
		{
		case ELynelCombatIndex::AimingBow: 
			eCombatIndex = ELynelCombatIndex::DashAttack;
			break;
		case ELynelCombatIndex::DashAttack:
			eCombatIndex = ELynelCombatIndex::ExplosionAttack;
			break;
		case ELynelCombatIndex::ExplosionAttack:
			eCombatIndex = ELynelCombatIndex::FireAttack;
			break;
		case ELynelCombatIndex::FireAttack:
			eCombatIndex = ELynelCombatIndex::AimingBowUpper;
			break;
		case ELynelCombatIndex::AimingBowUpper:
			eCombatIndex = ELynelCombatIndex::HornAttack;
			break;
		case ELynelCombatIndex::HornAttack:
			eCombatIndex = ELynelCombatIndex::RunningAttack;
			break;
		case ELynelCombatIndex::RunningAttack:
			eCombatIndex = ELynelCombatIndex::AimingBow;
			break;
		}
		break;


	case EMonsterState::FindWeapon:
		// Called when Bokoblin's Alert on
		// ignore it
		return;
		break;
	case EMonsterState::Dead:
		CharacterMonster->PlayMontage(EMonsterMontage::DEAD);
		break;
	case EMonsterState::Fire:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Eat:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Dance:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	case EMonsterState::Signal:
		break;
	case EMonsterState::AimingBow:
		if (CurrentWeapon)
		{


			// Holding Melee
			if (CurrentWeapon->GetWorldWeaponKind() != EWeaponKind::BOW)
			{
				CharacterMonster->PlayMontage(EMonsterMontage::SWORD_TO_BOW);
				ChangeState(EMonsterState::Temp);
				eNextState = EMonsterState::AimingBow;
			}
			// Holding Bow
			else
			{
				CharacterMonster->PlayMontage(EMonsterMontage::BOW_START);
			}
		}
		else
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DRAW_BOW);
			ChangeState(EMonsterState::Temp);
			eNextState = EMonsterState::AimingBow;
		}
		break;
	case EMonsterState::AimingBowUpper:
		if (CurrentWeapon)
		{
			// Holding Melee
			if (CurrentWeapon->GetWorldWeaponKind() != EWeaponKind::BOW)
			{
				CharacterMonster->PlayMontage(EMonsterMontage::SWORD_TO_BOW);
				ChangeState(EMonsterState::Temp);
				eNextState = EMonsterState::AimingBowUpper;
			}
			// Holding Bow
			else
			{
				CharacterMonster->PlayMontage(EMonsterMontage::BOW_UPPER_START);
			}
		}
		else
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DRAW_BOW);
			ChangeState(EMonsterState::Temp);
			eNextState = EMonsterState::AimingBowUpper;
		}
		break;
	case EMonsterState::DashAttack:
	{
		if (eReadyToAttackStep != EReadyToAttackStep::End)
		{
			eNextState = EMonsterState::RunningAttack;
			ChangeState(EMonsterState::ReadyToAttack);
			return;
		}

		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = CharacterMonster->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				FVector LeftDirection;
				LeftDirection.X = -Direction.Y;
				LeftDirection.Y = Direction.X;
				LeftDirection.Z = 0.f; // ignore z

				LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (LeftDirection * LYNEL_DASH_GOAL_OFFSET);

				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_DASH_SWORD_START);
			}
				break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = CharacterMonster->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				//FVector LeftDirection;
				//LeftDirection.X = -Direction.Y;
				//LeftDirection.Y = Direction.X;
				//LeftDirection.Z = 0.f; // ignore z

				//LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (Direction * LYNEL_DASH_GOAL_OFFSET);

				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_DASH_LSWORD_START);
			}
				break;
			case EWeaponKind::BOW:
				CharacterMonster->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
				eNextState = EMonsterState::DashAttack;
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected WeaponKind"));
				check(false);
				break;
			}
		}
		else
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DRAW_LSWORD);
			eNextState = EMonsterState::DashAttack;
			ChangeState(EMonsterState::Temp);
			return;
		}

	}
		break;
	case EMonsterState::ExplosionAttack:
	{
		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_EXPLOSION_START);
				break;
			case EWeaponKind::BOW:
				CharacterMonster->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
				eNextState = EMonsterState::ExplosionAttack;
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected WeaponKind"));
				check(false);
				break;
			}
		}
		else
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DRAW_LSWORD);
			eNextState = EMonsterState::ExplosionAttack;
			ChangeState(EMonsterState::Temp);
			return;
		}

	}
		break;
	case EMonsterState::FireAttack:
	{
		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_FIRE_START);
				break;
			case EWeaponKind::BOW:
				eNextState = EMonsterState::FireAttack;
				CharacterMonster->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected WeaponKind"));
				check(false);
				break;
			}
		}
		else
		{
			eNextState = EMonsterState::FireAttack;
			CharacterMonster->PlayMontage(EMonsterMontage::DRAW_LSWORD);
			ChangeState(EMonsterState::Temp);
			return;
		}

	}
		break;
	case EMonsterState::HornAttack:
	{
		if (eReadyToAttackStep != EReadyToAttackStep::End)
		{
			eNextState = EMonsterState::RunningAttack;
			ChangeState(EMonsterState::ReadyToAttack);
			return;
		}

		bHornAttackPassed = false;
		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				eNextState = EMonsterState::HornAttack;
				CharacterMonster->PlayMontage(EMonsterMontage::SHEATH_LSWORD);
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::BOW:
				eNextState = EMonsterState::HornAttack;
				CharacterMonster->PlayMontage(EMonsterMontage::SHEATH_BOW);
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected WeaponKind"));
				check(false);
				break;
			}
		}
		else
		{
			const FVector PlayerLocation = Player->GetActorLocation();
			const FVector MonsterLocation = CharacterMonster->GetActorLocation();

			FVector Direction = PlayerLocation - MonsterLocation;
			Direction.Z = 0.f; // ignore Z
			Direction.Normalize();

			TargetLocation = PlayerLocation + (Direction * LYNEL_DASH_GOAL_OFFSET);

			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_HORN_START);
		}

	}
	break;	
	case EMonsterState::RunningAttack:
	{
		if (eReadyToAttackStep != EReadyToAttackStep::End)
		{
			eNextState = EMonsterState::RunningAttack;
			ChangeState(EMonsterState::ReadyToAttack);
			return;
		}


		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = CharacterMonster->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				FVector LeftDirection;
				LeftDirection.X = -Direction.Y;
				LeftDirection.Y = Direction.X;
				LeftDirection.Z = 0.f; // ignore z

				LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (LeftDirection * LYNEL_DASH_GOAL_OFFSET);

				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_RUNNING_SWORD_START);
			}
			break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = CharacterMonster->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				//FVector LeftDirection;
				//LeftDirection.X = -Direction.Y;
				//LeftDirection.Y = Direction.X;
				//LeftDirection.Z = 0.f; // ignore z

				//LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (Direction * LYNEL_DASH_GOAL_OFFSET);

				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_RUNNING_LSWORD_START);
			}
				break;
			case EWeaponKind::BOW:
				CharacterMonster->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
				eNextState = EMonsterState::RunningAttack;
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected WeaponKind"));
				check(false);
				break;
			}
		}
		else
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DRAW_LSWORD);
			eNextState = EMonsterState::RunningAttack;
			ChangeState(EMonsterState::Temp);
			return;
		}

	}
		break;
	// CharacterMonster's Attack Blocked
	case EMonsterState::Rebound:
		CharacterMonster->PlayMontage(EMonsterMontage::REBOUND);
		break;
	case EMonsterState::Rodeo:
		CharacterMonster->PlayMontage(EMonsterMontage::RODEO_START);
		break;
	case EMonsterState::Stun:
		CharacterMonster->PlayMontage(EMonsterMontage::STUN_START);
		break;
	case EMonsterState::ReadyToAttack:
		break;
	case EMonsterState::Damage:
		return;
		break;
	case EMonsterState::Temp:
		break;
	default:
		break;
	}


	switch (NewState)
	{
	case EMonsterState::Idle:
	case EMonsterState::Suspicious:
	case EMonsterState::Dead:
		if (UWorld* World = CharacterMonster->GetWorld())
		{
			if (APC_InGame* PC = Cast<APC_InGame>(World->GetFirstPlayerController()))
			{
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					if (UMonsterStatusComponent* StatusComponent = CharacterMonster->GetStatusComponent())
					{
						HUD->ShowBossHpUI(false, StatusComponent->GetCurrentHP(), StatusComponent->GetMaxHP(), CharacterMonster->GetMonsterData()->Name.ToString());
					}
				}
			}
		}
		break;
	case EMonsterState::Alert:
	case EMonsterState::Combat:
	case EMonsterState::Fire:
	case EMonsterState::Signal:
	case EMonsterState::AimingBow:
	case EMonsterState::AimingBowUpper:
	case EMonsterState::DashAttack:
	case EMonsterState::ExplosionAttack:
	case EMonsterState::FireAttack:
	case EMonsterState::HornAttack:
	case EMonsterState::RunningAttack:
	case EMonsterState::Rebound:
	case EMonsterState::Rodeo:
	case EMonsterState::Stun:
	case EMonsterState::ReadyToAttack:
	case EMonsterState::Temp:
		if (UWorld* World = CharacterMonster->GetWorld())
		{
			if (APC_InGame* PC = Cast<APC_InGame>(World->GetFirstPlayerController()))
			{
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					if (UMonsterStatusComponent* StatusComponent = CharacterMonster->GetStatusComponent())
					{
						HUD->ShowBossHpUI(true, StatusComponent->GetCurrentHP(), StatusComponent->GetMaxHP(), CharacterMonster->GetMonsterData()->Name.ToString());
					}
				}
			}
		}
		break;
	default:
		break;
	}

	eCurrentState = NewState;
}

void ULynelFSMComponent::UpdateIdle(float DeltaTime)
{
	if (Player)
	{
		ChangeState(EMonsterState::Suspicious);
	}
}

void ULynelFSMComponent::UpdatePatrol(float DeltaTime)
{
	if (Player)
	{
		ChangeState(EMonsterState::Suspicious);
	}
}

void ULynelFSMComponent::UpdateSuspicious(float DeltaTime)
{
	if (Player)
	{
		this->StopMove();
		SuspicionGauge += DeltaTime * MONSTER_SUSPICIOUS_COEFFICIENT;

		const FVector MonsterLocation = CharacterMonster->GetActorLocation();
		const FVector PlayerLocation = Player->GetActorLocation();
		const float fDistance = FVector::Dist(MonsterLocation, PlayerLocation);

		if (SuspicionGauge >= MONSTER_MAX_SUSPICIOUS_GAUGE
			|| fDistance < MONSTER_IMMEDIATE_ALERT_RADIUS
			)
		{
			SuspicionGauge = 0.f;
			InstantRotateActorToDirection(CharacterMonster, PlayerLocation);
			ChangeState(EMonsterState::Alert);
			return;
		}
		else if (fDistance > MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS)
		{
			ChangeState(EMonsterState::Idle);
			Player = nullptr;
			return;
		}
		else
		{
			SuspicionGauge += DeltaTime;
		}

		SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime, 10.f);

	}
	else
	{
		ChangeState(EMonsterState::Idle);
		return;
	}
}

void ULynelFSMComponent::UpdateAlert(float DeltaTime)
{
}

void ULynelFSMComponent::UpdateCombat(float DeltaTime)
{
	// @TODO : Move To Player With RootMotionAnimation
	// 

	switch (eCombatIndex)
	{
	case ELynelCombatIndex::AimingBow:
		ChangeState(EMonsterState::AimingBow);
		return;
	case ELynelCombatIndex::DashAttack:
		ChangeState(EMonsterState::DashAttack);
		return;
	case ELynelCombatIndex::ExplosionAttack:
		ChangeState(EMonsterState::ExplosionAttack);
		return;
	case ELynelCombatIndex::FireAttack:
		ChangeState(EMonsterState::FireAttack);
		return;
	case ELynelCombatIndex::AimingBowUpper:
		ChangeState(EMonsterState::AimingBowUpper);
		return;
	case ELynelCombatIndex::HornAttack:
		ChangeState(EMonsterState::HornAttack);
		return;
	case ELynelCombatIndex::RunningAttack:
		ChangeState(EMonsterState::RunningAttack);
		return;
	case ELynelCombatIndex::End:
	default:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::UpdateCombat // Unexpected CombatIndex"));
		check(false);
		break;
	}
}

void ULynelFSMComponent::UpdateAimingBow(float DeltaTime)
{
	if (!Player)
	{
		AimingBowElapsedTime = 0.f;
		ChangeState(EMonsterState::Idle);
		return;
	}


	this->StopMove();
	AimingBowElapsedTime += DeltaTime;
	const FVector PlayerLocation = Player->GetActorLocation();
	SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);

	if (AimingBowElapsedTime > MONSTER_AIMINGBOW_MAX_TIME)
	{
		AimingBowElapsedTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::BOW_END);
	}
}

void ULynelFSMComponent::UpdateAimingBowUpper(float DeltaTime)
{
	if (!Player)
	{
		AimingBowElapsedTime = 0.f;
		ChangeState(EMonsterState::Idle);
		return;
	}

	this->StopMove();
	AimingBowElapsedTime += DeltaTime;
	const FVector PlayerLocation = Player->GetActorLocation();
	SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);

	if (AimingBowElapsedTime > MONSTER_AIMINGBOW_MAX_TIME)
	{
		AimingBowElapsedTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::BOW_UPPER_END);
	}

}

void ULynelFSMComponent::UpdateDashAttack(float DeltaTime)
{
	// In this case, CharacterMonster will dash to Link's left position

	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = CharacterMonster->GetActorLocation();

	if (CharacterMonster->IsPlayingMontage(EMonsterMontage::END))
	{
		this->StopMove();
	}
	else
	{
		MoveToLocation(PlayerLocation);
	}


	const bool bIsNear = FVector::PointsAreNear(MonsterLocation, PlayerLocation, MONSTER_DEFAULT_NEAR_DISTANCE);

	if (bIsNear)
	{
		if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::END))
		{
			if (CurrentWeapon)
			{
				EWeaponKind WeaponKind = CurrentWeapon->GetWorldWeaponKind();
				switch (WeaponKind)
				{
				case EWeaponKind::SWORD:
					CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_DASH_SWORD_END);
					break;
				case EWeaponKind::SPEAR:
				case EWeaponKind::LSWORD:
					CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_DASH_LSWORD_END);
					break;
				case EWeaponKind::BOW:
				case EWeaponKind::END:
				default:
					UE_LOG(LogTemp, Error, TEXT("LynelFSMComponent::UpdateDashAttack // Unexpected Weapon Kind"));
					check(false);
					return;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("LynelFSMComponent::UpdateDashAttack // No CurrentWeapon"));
				check(false);
				return;
			}
		}
	}
}

void ULynelFSMComponent::UpdateExplosionAttack(float DeltaTime)
{
	// Do Nothing
	// Pattern will be handled in AnimNotify
	this->StopMove();
}

void ULynelFSMComponent::UpdateFireAttack(float DeltaTime)
{
	if (!Player)
	{
		AimingBowElapsedTime = 0.f;
		ChangeState(EMonsterState::Idle);
		return;
	}

	this->StopMove();
	const FVector PlayerLocation = Player->GetActorLocation();
	SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);
}

void ULynelFSMComponent::UpdateHornAttack(float DeltaTime)
{
	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = CharacterMonster->GetActorLocation();

	if (CharacterMonster->IsPlayingMontage(EMonsterMontage::ATTACK_HORN_START))
	{
		this->StopMove();
	}
	else
	{
		MoveToLocation(PlayerLocation);
	}


	if (bHornAttackPassed)
	{
		if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::ATTACK_HORN_END))
		{
			CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_HORN_END);
		}
		return;
		//const float fDist = FVector::Dist(PlayerLocation, MonsterLocation);
		//if (fDist > LYNEL_HORN_ATTACK_MAX_PASS_LENGTH)
		//{
		//	// End
		//	bHornAttackPassed = false;
		//	CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_HORN_END);
		//	return;
		//}
	}

	const bool bIsNear = FVector::PointsAreNear(MonsterLocation, PlayerLocation, MONSTER_DEFAULT_NEAR_DISTANCE);

	if (bIsNear)
	{
		bHornAttackPassed = true;
	}
}

void ULynelFSMComponent::UpdateRunningAttack(float DeltaTime)
{
	// In this case, CharacterMonster will dash to Link's left position

	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = CharacterMonster->GetActorLocation();


	FVector Direction = PlayerLocation - MonsterLocation;
	Direction.Z = 0.f; // ignore Z
	Direction.Normalize();

	FVector LeftDirection;
	LeftDirection.X = -Direction.Y;
	LeftDirection.Y = Direction.X;
	LeftDirection.Z = 0.f; // ignore z

	LeftDirection.Normalize();

	TargetLocation = PlayerLocation + (LeftDirection * LYNEL_DASH_GOAL_OFFSET);

	MoveToLocation(TargetLocation);

	const bool bIsNear = FVector::PointsAreNear(PlayerLocation, MonsterLocation, MONSTER_DEFAULT_NEAR_DISTANCE);

	if (bIsNear)
	{
		if (CurrentWeapon)
		{
			EWeaponKind WeaponKind = CurrentWeapon->GetWorldWeaponKind();
			switch (WeaponKind)
			{
			case EWeaponKind::SWORD:
				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_RUNNING_SWORD_END);
				break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_RUNNING_LSWORD_END);
				break;
			case EWeaponKind::BOW:
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("LynelFSMComponent::UpdateDashAttack // Unexpected Weapon Kind"));
				check(false);
				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LynelFSMComponent::UpdateDashAttack // No CurrentWeapon"));
			check(false);
			return;
		}
	}
}

void ULynelFSMComponent::UpdateRebound(float DeltaTime)
{
	// Pattern will be handled in AnimNotify
}

void ULynelFSMComponent::UpdateRodeo(float DeltaTime)
{
	RodeoElapsedTime += DeltaTime;
	if (RodeoElapsedTime > LYNEL_RODEO_MAX_TIME)
	{
		RodeoElapsedTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::RODEO_END);
	}
}

void ULynelFSMComponent::UpdateStun(float DeltaTime)
{
	StunElapsedTime += DeltaTime;

	if (StunElapsedTime > LYNEL_STUN_MAX_TIME)
	{
		StunElapsedTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::STUN_END);
	}
}

void ULynelFSMComponent::UpdateReadyToAttack(float DeltaTime)
{
	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		eReadyToAttackStep = EReadyToAttackStep::RunToLink;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = CharacterMonster->GetActorLocation();

	switch (eReadyToAttackStep)
	{
	case EReadyToAttackStep::RunToLink:
	{
		SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);
		if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::GEAR_3_FORWARD))
		{
			CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_FORWARD);
		}
		const bool bIsNear = FVector::PointsAreNear(PlayerLocation, MonsterLocation, 300.f);
		if (bIsNear)
		{
			eReadyToAttackStep = EReadyToAttackStep::TurnRight;
			if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::GEAR_3_RIGHT))
			{
				UE_LOG(LogTemp, Warning, TEXT("ULynelFSMComponent::UpdateReadyToAttack // CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_RIGHT)"));
				CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_RIGHT);
			}
		}
	}	
		break;
	case EReadyToAttackStep::TurnRight:
		if (LyenlTurnRightCount >= 5)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_FORWARD);
			LyenlTurnRightCount = 0;
			eReadyToAttackStep = EReadyToAttackStep::AwayFromLink;
			UE_LOG(LogTemp, Warning, TEXT("Changed to EReadyToAttackStep::AwayFromLink"));
		}
		else
		{
			bool bFlag = CharacterMonster->IsPlayingMontage(EMonsterMontage::GEAR_3_RIGHT);
			if (!bFlag)
			{
				UE_LOG(LogTemp, Warning, TEXT("ULynelFSMComponent::UpdateReadyToAttack // CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_RIGHT)"));
				CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_RIGHT);
			}
		}
		break;
	case EReadyToAttackStep::AwayFromLink:
	{

		FVector DirectionToPlayer = PlayerLocation - MonsterLocation;
		DirectionToPlayer.Z = 0.f; // ignore Z
		FVector NormalizedDirectionToPlayer = DirectionToPlayer.GetSafeNormal();
		FVector DirectionAwayFromPlayer = -NormalizedDirectionToPlayer;
		float DistanceToMoveAway = LYNEL_AWAY_FROM_LINK_OFFSET;
		FVector AwayLocation = MonsterLocation + (DirectionAwayFromPlayer * DistanceToMoveAway);
		SmoothRotateActorToDirection(CharacterMonster, AwayLocation, DeltaTime);

		if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::GEAR_3_FORWARD))
		{
			CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_FORWARD);
		}

		float fDistance = FVector::Dist(MonsterLocation, PlayerLocation);
		if (fDistance > LYNEL_AWAY_FROM_LINK_OFFSET)
		{
			eReadyToAttackStep = EReadyToAttackStep::TurnLeft;
			UE_LOG(LogTemp, Warning, TEXT("Changed to EReadyToAttackStep::TurnLeft"));

			if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::GEAR_3_LEFT))
			{
				UE_LOG(LogTemp, Warning, TEXT("ULynelFSMComponent::UpdateReadyToAttack // CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_LEFT)"));
				CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_LEFT);
			}
		}

	}
		break;
	case EReadyToAttackStep::TurnLeft:
		if (LyenlTurnLeftCount >= 5)
		{
			LyenlTurnLeftCount = 0;
			eReadyToAttackStep = EReadyToAttackStep::End;
			ChangeState(eNextState); // Horn or Dash or Running Attack
			UE_LOG(LogTemp, Warning, TEXT("Changed to EReadyToAttackStep::End"));
		}
		else
		{
			if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::GEAR_3_LEFT))
			{
				UE_LOG(LogTemp, Warning, TEXT("ULynelFSMComponent::UpdateReadyToAttack // CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_LEFT)"));
				CharacterMonster->PlayMontage(EMonsterMontage::GEAR_3_LEFT);
			}
		}
		break;
	case EReadyToAttackStep::End:
		break;
	default:
		break;
	}


}

void ULynelFSMComponent::UpdateTemp(float DeltaTime)
{
	// This state is a state to wait until AnimNontage end
	// Do nothing in here
}
