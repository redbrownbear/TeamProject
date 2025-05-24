// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

#include "Actors/Monster/Monster.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"


ULynelFSMComponent::ULynelFSMComponent()
{
	eCurrentState = EMonsterState::Idle;
}

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
	case EMonsterState::Dead:
		UpdateDead(DeltaTime);
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
		break;
	case EMonsterState::ExplosionAttack:
		break;
	case EMonsterState::FireAttack:
		break;
	case EMonsterState::HornAttack:
		break;
	case EMonsterState::RunningAttack:
		break;
	case EMonsterState::Rebound:
		break;
	case EMonsterState::Rodeo:
		break;
	case EMonsterState::Stun:
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
		float ReturnValue = GetSideOfActor(Owner, Player);
		// RightSide
		if (ReturnValue > 0.f)
			Owner->PlayMontage(EMonsterMontage::TURN_180_R);
		else
			Owner->PlayMontage(EMonsterMontage::TURN_180_L);
	}
		break;
	case EMonsterState::Alert:
		Owner->PlayMontage(EMonsterMontage::SIGNAL_START);
		// @TODO : Effect
		break;
	case EMonsterState::Combat:
		// To show Combat Pattern in order
		switch (eCombatIndex)
		{
		case ECombatIndex::AimingBow: 
			eCombatIndex = ECombatIndex::DashAttack;
			break;
		case ECombatIndex::DashAttack:
			eCombatIndex = ECombatIndex::ExplosionAttack;
			break;
		case ECombatIndex::ExplosionAttack:
			eCombatIndex = ECombatIndex::FireAttack;
			break;
		case ECombatIndex::FireAttack:
			eCombatIndex = ECombatIndex::AimingBowUpper;
			break;
		case ECombatIndex::AimingBowUpper:
			eCombatIndex = ECombatIndex::HornAttack;
			break;
		case ECombatIndex::HornAttack:
			eCombatIndex = ECombatIndex::RunningAttack;
			break;
		case ECombatIndex::RunningAttack:
			eCombatIndex = ECombatIndex::AimingBow;
			break;
		}
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
		if (CurrentWeapon)
		{
			// Holding Melee
			if (CurrentWeapon->GetWorldWeaponKind() != EWeaponKind::BOW)
			{
				Owner->PlayMontage(EMonsterMontage::SWORD_TO_BOW);
				ChangeState(EMonsterState::Temp);
				eNextState = EMonsterState::AimingBow;
			}
			// Holding Bow
			else
			{
				Owner->PlayMontage(EMonsterMontage::BOW_START);
			}
		}
		else
		{
			Owner->PlayMontage(EMonsterMontage::DRAW_BOW);
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
				Owner->PlayMontage(EMonsterMontage::SWORD_TO_BOW);
				ChangeState(EMonsterState::Temp);
				eNextState = EMonsterState::AimingBowUpper;
			}
			// Holding Bow
			else
			{
				Owner->PlayMontage(EMonsterMontage::BOW_UPPER_START);
			}
		}
		else
		{
			Owner->PlayMontage(EMonsterMontage::DRAW_BOW);
			ChangeState(EMonsterState::Temp);
			eNextState = EMonsterState::AimingBowUpper;
		}
		break;
	case EMonsterState::DashAttack:
	{
		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = Owner->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				FVector LeftDirection;
				LeftDirection.X = -Direction.Y;
				LeftDirection.Y = Direction.X;
				LeftDirection.Z = 0.f; // ignore z

				LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (LeftDirection * LYNEL_DASH_GOAL_OFFSET);

				Owner->PlayMontage(EMonsterMontage::ATTACK_DASH_SWORD_START);
			}
				break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = Owner->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				//FVector LeftDirection;
				//LeftDirection.X = -Direction.Y;
				//LeftDirection.Y = Direction.X;
				//LeftDirection.Z = 0.f; // ignore z

				//LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (Direction * LYNEL_DASH_GOAL_OFFSET);

				Owner->PlayMontage(EMonsterMontage::ATTACK_DASH_LSWORD_START);
			}
				break;
			case EWeaponKind::BOW:
				Owner->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
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
			Owner->PlayMontage(EMonsterMontage::DRAW_LSWORD);
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
				Owner->PlayMontage(EMonsterMontage::ATTACK_EXPLOSION_START);
				break;
			case EWeaponKind::BOW:
				Owner->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
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
			Owner->PlayMontage(EMonsterMontage::DRAW_LSWORD);
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
				Owner->PlayMontage(EMonsterMontage::ATTACK_FIRE_START);
				break;
			case EWeaponKind::BOW:
				eNextState = EMonsterState::FireAttack;
				Owner->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
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
			Owner->PlayMontage(EMonsterMontage::DRAW_LSWORD);
			ChangeState(EMonsterState::Temp);
			return;
		}

	}
		break;
	case EMonsterState::HornAttack:
	{
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
				Owner->PlayMontage(EMonsterMontage::SHEATH_LSWORD);
				ChangeState(EMonsterState::Temp);
				return;
			case EWeaponKind::BOW:
				eNextState = EMonsterState::HornAttack;
				Owner->PlayMontage(EMonsterMontage::SHEATH_BOW);
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
			const FVector MonsterLocation = Owner->GetActorLocation();

			FVector Direction = PlayerLocation - MonsterLocation;
			Direction.Z = 0.f; // ignore Z
			Direction.Normalize();

			TargetLocation = PlayerLocation + (Direction * LYNEL_DASH_GOAL_OFFSET);

			Owner->PlayMontage(EMonsterMontage::ATTACK_HORN_START);
		}

	}
	break;	
	case EMonsterState::RunningAttack:
	{
		if (CurrentWeapon)
		{
			EWeaponKind eKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eKind)
			{
			case EWeaponKind::SWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = Owner->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				FVector LeftDirection;
				LeftDirection.X = -Direction.Y;
				LeftDirection.Y = Direction.X;
				LeftDirection.Z = 0.f; // ignore z

				LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (LeftDirection * LYNEL_DASH_GOAL_OFFSET);

				Owner->PlayMontage(EMonsterMontage::ATTACK_RUNNING_SWORD_START);
			}
			break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector MonsterLocation = Owner->GetActorLocation();

				FVector Direction = PlayerLocation - MonsterLocation;
				Direction.Z = 0.f; // ignore Z
				Direction.Normalize();

				//FVector LeftDirection;
				//LeftDirection.X = -Direction.Y;
				//LeftDirection.Y = Direction.X;
				//LeftDirection.Z = 0.f; // ignore z

				//LeftDirection.Normalize();

				TargetLocation = PlayerLocation + (Direction * LYNEL_DASH_GOAL_OFFSET);

				Owner->PlayMontage(EMonsterMontage::ATTACK_RUNNING_LSWORD_START);
			}
				break;
			case EWeaponKind::BOW:
				Owner->PlayMontage(EMonsterMontage::BOW_TO_SWORD);
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
			Owner->PlayMontage(EMonsterMontage::DRAW_LSWORD);
			eNextState = EMonsterState::RunningAttack;
			ChangeState(EMonsterState::Temp);
			return;
		}

	}
		break;
	// Monster's Attack Blocked
	case EMonsterState::Rebound:
		Owner->PlayMontage(EMonsterMontage::REBOUND);
		break;
	case EMonsterState::Rodeo:
		Owner->PlayMontage(EMonsterMontage::RODEO_START);
		break;
	case EMonsterState::Stun:
		Owner->PlayMontage(EMonsterMontage::STUN_START);
		break;
	case EMonsterState::Temp:
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

		const FVector MonsterLocation = Owner->GetActorLocation();
		const FVector PlayerLocation = Player->GetActorLocation();
		const float fDistance = FVector::Dist(MonsterLocation, PlayerLocation);

		if (SuspicionGauge >= MONSTER_MAX_SUSPICIOUS_GAUGE
			|| fDistance < MONSTER_IMMEDIATE_ALERT_RADIUS
			)
		{
			SuspicionGauge = 0.f;
			InstantRotateActorToDirection(Owner, PlayerLocation);
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

		SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime, 10.f);

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
	case ECombatIndex::AimingBow:
		ChangeState(EMonsterState::AimingBow);
		return;
	case ECombatIndex::DashAttack:
		ChangeState(EMonsterState::DashAttack);
		return;
	case ECombatIndex::ExplosionAttack:
		ChangeState(EMonsterState::ExplosionAttack);
		return;
	case ECombatIndex::FireAttack:
		ChangeState(EMonsterState::FireAttack);
		return;
	case ECombatIndex::AimingBowUpper:
		ChangeState(EMonsterState::AimingBowUpper);
		return;
	case ECombatIndex::HornAttack:
		ChangeState(EMonsterState::HornAttack);
		return;
	case ECombatIndex::RunningAttack:
		ChangeState(EMonsterState::RunningAttack);
		return;
	case ECombatIndex::End:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::UpdateCombat // Unexpected CombatIndex"));
		check(false);
	default:
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
	SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);

	if (AimingBowElapsedTime > MONSTER_AIMINGBOW_MAX_TIME)
	{
		AimingBowElapsedTime = 0.f;
		Owner->PlayMontage(EMonsterMontage::BOW_END);
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
	SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);

	if (AimingBowElapsedTime > MONSTER_AIMINGBOW_MAX_TIME)
	{
		AimingBowElapsedTime = 0.f;
		Owner->PlayMontage(EMonsterMontage::BOW_UPPER_END);
	}

}

void ULynelFSMComponent::UpdateDashAttack(float DeltaTime)
{
	// In this case, Lynel will dash to Link's left position

	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = Owner->GetActorLocation();

	if (Owner->IsPlayingMontage(EMonsterMontage::END))
	{
		this->StopMove();
	}
	else
	{
		MoveToLocation(PlayerLocation);
	}


	const bool bIsNear = FVector::PointsAreNear(MonsterLocation, PlayerLocation, 150.f);

	if (bIsNear)
	{
		if (CurrentWeapon)
		{
			EWeaponKind WeaponKind = CurrentWeapon->GetWorldWeaponKind();
			switch (WeaponKind)
			{
			case EWeaponKind::SWORD:
				Owner->PlayMontage(EMonsterMontage::ATTACK_DASH_SWORD_END);
				break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				Owner->PlayMontage(EMonsterMontage::ATTACK_DASH_LSWORD_END);
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
	SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);
}

void ULynelFSMComponent::UpdateHornAttack(float DeltaTime)
{
	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = Owner->GetActorLocation();

	if (Owner->IsPlayingMontage(EMonsterMontage::ATTACK_HORN_START))
	{
		this->StopMove();
	}
	else
	{
		MoveToLocation(PlayerLocation);
	}


	if (bHornAttackPassed)
	{
		Owner->PlayMontage(EMonsterMontage::ATTACK_HORN_END);
		return;
		const float fDist = FVector::Dist(PlayerLocation, MonsterLocation);
		//if (fDist > LYNEL_HORN_ATTACK_MAX_PASS_LENGTH)
		//{
		//	// End
		//	bHornAttackPassed = false;
		//	Owner->PlayMontage(EMonsterMontage::ATTACK_HORN_END);
		//	return;
		//}
	}

	const bool bIsNear = FVector::PointsAreNear(MonsterLocation, PlayerLocation, 150.f);

	if (bIsNear)
	{
		bHornAttackPassed = true;
	}
}

void ULynelFSMComponent::UpdateRunningAttack(float DeltaTime)
{
	// In this case, Lynel will dash to Link's left position

	if (!Player)
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = Owner->GetActorLocation();


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

	const bool bIsNear = FVector::PointsAreNear(PlayerLocation, MonsterLocation, 150.f);

	if (bIsNear)
	{
		if (CurrentWeapon)
		{
			EWeaponKind WeaponKind = CurrentWeapon->GetWorldWeaponKind();
			switch (WeaponKind)
			{
			case EWeaponKind::SWORD:
				Owner->PlayMontage(EMonsterMontage::ATTACK_RUNNING_SWORD_END);
				break;
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				Owner->PlayMontage(EMonsterMontage::ATTACK_RUNNING_LSWORD_END);
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

void ULynelFSMComponent::UpdateDead(float DeltaTime)
{
	// Pattern will be handled in AnimNotify
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
		Owner->PlayMontage(EMonsterMontage::RODEO_END);
	}
}

void ULynelFSMComponent::UpdateStun(float DeltaTime)
{
	StunElapsedTime += DeltaTime;

	if (StunElapsedTime > LYNEL_STUN_MAX_TIME)
	{
		StunElapsedTime = 0.f;
		Owner->PlayMontage(EMonsterMontage::STUN_END);
	}
}

void ULynelFSMComponent::UpdateTemp(float DeltaTime)
{
	// This state is a state to wait until AnimNontage end
	// Do nothing in here
}
