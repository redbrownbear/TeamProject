// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/HinoxAnimInstance.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

#include "Subsystem/TimeManager.h"

UHinoxAnimInstance::UHinoxAnimInstance()
{
}

void UHinoxAnimInstance::NativeInitializeAnimation()
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
		FSMComponent = Cast<UHinoxFSMComponent>(Monster->GetFSMComponent());
		TimeManager = Monster->GetTimeManagerSubsystem();
	}
}

void UHinoxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::NativeUpdateAnimation(CustumDeltaTime);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UHinoxFSMComponent>(Monster->GetFSMComponent());
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
	bIsDamageEye = false;

	switch (eMonsterState)
	{
	case EMonsterState::Idle:
		bIsIdle = true;
		break;
	case EMonsterState::Damage_Eye:
		bIsDamageEye = true;
		break;
	default:
		break;
	}

}
