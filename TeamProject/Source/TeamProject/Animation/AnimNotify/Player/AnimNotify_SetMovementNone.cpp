// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SetMovementNone.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_SetMovementNone::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	ACharacter* Player_C = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Player_C)
		return;
	Player_C->GetCharacterMovement()->SetMovementMode(MOVE_None);
}
