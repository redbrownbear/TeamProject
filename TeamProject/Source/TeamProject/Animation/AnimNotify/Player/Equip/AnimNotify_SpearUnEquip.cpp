// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/Equip/AnimNotify_SpearUnEquip.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"

void UAnimNotify_SpearUnEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}

	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInst) 
	{
		AnimInst->Equip_State = EEquip_State::None;
	}
}
