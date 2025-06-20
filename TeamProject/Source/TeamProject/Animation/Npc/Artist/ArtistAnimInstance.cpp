// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Npc/Artist/ArtistAnimInstance.h"
#include "Actors/Npc/Npc.h"

UArtistAnimInstance::UArtistAnimInstance()
	:Super()
{
}

void UArtistAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();

	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UArtistAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	ANpc* Npc = Cast<ANpc>(Pawn);
	ArtistFSMComponent = Cast<UArtistFSMComponent>(Npc->GetFSMComponent());
}

void UArtistAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ANpc* Npc = Cast<ANpc>(TryGetPawnOwner()))
	{
		ArtistFSMComponent = Cast<UArtistFSMComponent>(Npc->GetFSMComponent());
	}

	if (!ArtistFSMComponent) { return; }

	if (APawn* Pawn = TryGetPawnOwner())
	{
		Speed = Pawn->GetVelocity().Size2D();
	}

	eCurrentState = ArtistFSMComponent->GetNpcState();

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