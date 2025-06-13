// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotifySetMMMW.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "AnimNotifySetMMMW.h"

void UAnimNotifySetMMMW::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}
	UPlayerMovementComponent* Movement= Cast<UPlayerMovementComponent>(PlayerCharacter->GetCharacterMovement());
	EMove_State Move_State = Movement->GetMoveState();
	if (Move_State == EMove_State::Climb)
	{
		Movement->SetClimbMode(false);
	}
}
