// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/AssasinLeaderAnimInstance.h"
#include "Components/FSMComponent/Monster/AssasinLeaderFSMComponent.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

UAssasinLeaderAnimInstance::UAssasinLeaderAnimInstance()
{
}

void UAssasinLeaderAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("To use UHinoxAnimInstance, Owner muse be Pawn."));
		return;
	}
	else if (!Pawn) { return; }

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(Pawn))
	{
		FSMComponent = Cast<UAssasinLeaderFSMComponent>(Monster->GetFSMComponent());
	}
}

void UAssasinLeaderAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UAssasinLeaderFSMComponent>(Monster->GetFSMComponent());
	}
	if (!FSMComponent) return;
	const EMonsterState eMonsterState = FSMComponent->GetMonsterState();
	bIsIdle = false;
	bIsCombatIdle = false;
	bIsWalk = false;
	bIsCombatWalk = false;
	bIsHappyWalk = false;

	switch (eMonsterState)
	{
	case EMonsterState::Idle:
		bIsIdle = true;
		break;
	case EMonsterState::Combat:
		if (ACharacterMonster* Monster = Cast<ACharacterMonster>(TryGetPawnOwner()))
		{
			const double Speed = UKismetMathLibrary::VSizeXY(Monster->GetMovementComponent()->Velocity);
			if (FMath::IsNearlyZero(Speed))
			{
				bIsCombatIdle = true;
			}
			else
			{
				bIsCombatWalk = true;
			}
		}
		break;
	case EMonsterState::Suspicious:
		bIsSuspicious = true;
		break;
	case EMonsterState::Patrol:
		bIsWalk = true;
		break;
	case EMonsterState::Happy:
		bIsHappyWalk = true;
		break;
	default:
		break;
	}
}
