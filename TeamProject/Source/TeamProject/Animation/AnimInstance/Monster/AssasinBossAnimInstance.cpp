// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/AssasinBossAnimInstance.h"
#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/Utils.h"

UAssasinBossAnimInstance::UAssasinBossAnimInstance()
{
}

void UAssasinBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("To use UAssasinBossAnimInstance, Owner muse be Pawn."));
		return;
	}
	else if (!Pawn) { return; }

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(Pawn))
	{
		FSMComponent = Cast<UAssasinBossFSMComponent>(Monster->GetFSMComponent());
	}
}

void UAssasinBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UAssasinBossFSMComponent>(Monster->GetFSMComponent());
	}
	if (!FSMComponent) return;
	const EMonsterState eMonsterState = FSMComponent->GetMonsterState();


	bIsIdle = false;
	bIsFlyIdle = false;
	bIsStun = false;
	bIsStoneFirst = false;
	bIsStoneSecond = false;
	bIsBarrier = false;
	bIsDamage = false;

	switch (eMonsterState)
	{
	case EMonsterState::Idle:
	case EMonsterState::Combat:
	{
		const EAssasinBossPhase Phase = FSMComponent->GetPhase();
		switch (Phase)
		{
		case EAssasinBossPhase::PHASE_1:
			bIsIdle = true;
			break;
		case EAssasinBossPhase::PHASE_3:
		case EAssasinBossPhase::PHASE_2:
			bIsFlyIdle = true;
			break;
		}
		break;
	}
	case EMonsterState::Barrier:
		bIsBarrier = true;
		break;
	case EMonsterState::Stone: 
	{
		const EAssasinBossPhase Phase = FSMComponent->GetPhase();
		switch (Phase)
		{
		case EAssasinBossPhase::PHASE_1:
		case EAssasinBossPhase::PHASE_3:
			bIsStoneFirst = true;
			break;
		case EAssasinBossPhase::PHASE_2:
			bIsStoneSecond = true;
			break;
		}
	}
		break;
	case EMonsterState::Stun:
		bIsStun = true;
		break;
	case EMonsterState::Damage:
		bIsDamage = true;
		break;
	default:
		break;
	}

}
