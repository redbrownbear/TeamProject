// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_HitEnd.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "Components/Character/WeaponManagerComponent.h"

void UAnimNotify_HitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(PlayerCharacter->GetCharacterMovement());
	Movement->SetMoveState(EMove_State::Run);
	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();
	Movement->SetMovementMode(MOVE_Walking);
	WeaponManagerComponent->SetCanSwordAttack();

}
