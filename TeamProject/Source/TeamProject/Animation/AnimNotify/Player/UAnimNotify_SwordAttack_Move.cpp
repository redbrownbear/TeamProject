// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/UAnimNotify_SwordAttack_Move.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/AttackComponent/WeaponComponent.h"


void UUAnimNotify_SwordAttack_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (Player_C)
	{
		UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
		Movement->SetMoveState(EMove_State::Run);
	}

}
