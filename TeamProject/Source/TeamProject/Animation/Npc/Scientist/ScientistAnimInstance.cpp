// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Npc/Scientist/ScientistAnimInstance.h"
#include "Actors/Npc/Npc.h"

UScientistAnimInstance::UScientistAnimInstance()
	:Super()
{
}

void UScientistAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();

	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UScientistAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	ANpc* Npc = Cast<ANpc>(Pawn);
	ScientistFSMComponent = Cast<UScientistFSMComponent>(Npc->GetFSMComponent());
}

void UScientistAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ANpc* Npc = Cast<ANpc>(TryGetPawnOwner()))
	{
		ScientistFSMComponent = Cast<UScientistFSMComponent>(Npc->GetFSMComponent());
	}

	if (!ScientistFSMComponent) { return; }

	if (APawn* Pawn = TryGetPawnOwner())
	{
		Speed = Pawn->GetVelocity().Size2D();
	}

	eCurrentState = ScientistFSMComponent->GetNpcState();

	bIsIdle = false;
	bIsSit = false;
	bIsStand = false;
	bIsWalk = false;
	bIsRun = false;
	bIsTalk = false;
	bIsHide = false;

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		bIsIdle = true;
		break;
	case ENpcState::Walk:
		bIsWalk = true;
		break;
	default:
		break;
	}
}