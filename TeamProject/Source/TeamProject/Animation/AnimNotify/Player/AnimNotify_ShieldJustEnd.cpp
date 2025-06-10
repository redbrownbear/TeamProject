// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_ShieldJustEnd.h"
#include "Actors/Character/PlayerCharacter.h"


void UAnimNotify_ShieldJustEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (Player_C)
	{

		Player_C->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		Player_C->GetWeaponManagerComponent()->SetIsGuard(false);
	}
}
