// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/UAnimNotify_SwordAttack_Move.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/AttackComponent/SwordComponent.h"


void UUAnimNotify_SwordAttack_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (Player_C)
	{
		Player_C->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

}
