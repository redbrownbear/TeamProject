// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_GlideVisible.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_GlideVisible::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
	PlayerCharacter->GetWeaponManagerComponent()->GetGlider()->SetVisibleGldier(true);
}
