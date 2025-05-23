// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/LynelAnimInstance.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

#include "Actors/Monster/Monster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

ULynelAnimInstance::ULynelAnimInstance()
{
}

void ULynelAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("To use ULynelAnimInstance, Owner muse be Pawn."));
		return;
	}
	else if (!Pawn) { return; }

	AMonster* Monster = Cast<AMonster>(Pawn);
	FSMComponent = Cast<ULynelFSMComponent>(Monster->GetFSMComponent());
}

void ULynelAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (AMonster* Monster = Cast<AMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<ULynelFSMComponent>(Monster->GetFSMComponent());
	}

	if (!FSMComponent) return;

	const EMonsterState eMonsterState = FSMComponent->GetMonsterState();

	bIsIdle = true;
	bIsWalk = false;
	bIsRun = false;
	bIsSearch = false;
	bIsFire = false;
	bIsEat = false;
	bIsDance = false;
	bIsSuspicious = false;
	bIsAimingBow = false;
	bIsAimingBowUpper = false;
	bIsAttackRunning = false;
	bIsAttackHorn = false;
	bIsAttackDash = false;
	bIsRodeo = false;
	bIsStun = false;

	switch (eMonsterState)
	{
	case EMonsterState::Idle:
		bIsIdle = true;
		break;
	case EMonsterState::Patrol:
		bIsWalk = true;
		break;
	case EMonsterState::Suspicious:
		bIsSuspicious = true;
		break;
	case EMonsterState::Alert:
		check(false);
		break;
	case EMonsterState::Combat:
		bIsRun = true;
		break;
	case EMonsterState::FindWeapon:
		check(false);
		break;
	case EMonsterState::Dead:
		break;
	case EMonsterState::Fire:
		check(false);
		break;
	case EMonsterState::Eat:
		check(false);
		break;
	case EMonsterState::Dance:
		check(false);
		break;
	case EMonsterState::Signal:
		check(false);
		break;
	case EMonsterState::AimingBow:
		bIsAimingBow = true;
		break;
	case EMonsterState::AimingBowUpper:
		bIsAimingBowUpper = true;
		break;
	case EMonsterState::DashAttack:
		bIsAttackDash = true;
		break;
	case EMonsterState::ExplosionAttack:
		bIsIdle = true;
		break;
	case EMonsterState::FireAttack:
		bIsIdle = true;
		break;
	case EMonsterState::HornAttack:
		bIsAttackHorn = true;
		break;
	case EMonsterState::RunningAttack:
		bIsAttackRunning = true;
		break;
	case EMonsterState::Rebound:
		bIsIdle = true;
		break;
	case EMonsterState::Rodeo:
		bIsRodeo = true;
		break;
	case EMonsterState::Stun:
		bIsStun = true;
		break;
	default:
		break;
	}

}
