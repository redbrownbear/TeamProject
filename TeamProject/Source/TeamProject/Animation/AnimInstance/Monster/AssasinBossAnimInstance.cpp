// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/AssasinBossAnimInstance.h"
#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/Utils.h"

#include "Subsystem/TimeManager.h"

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
		TimeManager = Monster->GetTimeManagerSubsystem();
	}
}

void UAssasinBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::NativeUpdateAnimation(CustumDeltaTime);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UAssasinBossFSMComponent>(Monster->GetFSMComponent());
	}
	if (!FSMComponent) return;

	UAnimMontage* CurrentMontage = GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		float RateScale = CurrentMontage->RateScale;
		float TimeManagerScale = TimeManager->GetTimeScale();
		Montage_SetPlayRate(CurrentMontage, RateScale * TimeManagerScale);
	}

	const EMonsterState eMonsterState = FSMComponent->GetCurrentState();

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
