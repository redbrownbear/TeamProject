// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/HinoxAnimInstance.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

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
	}
}

void UHinoxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UHinoxFSMComponent>(Monster->GetFSMComponent());
	}

	if (!FSMComponent) return;

	const EMonsterState eMonsterState = FSMComponent->GetMonsterState();

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
