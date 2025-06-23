// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/Equip/AnimNotify_SpearEquip.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_SpearEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (!MeshComp)
	{
		return;
	}

	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Equip_State = EEquip_State::Spear;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();
	WeaponManagerComponent->SetEquipState(EEquip_State::Spear);
}
