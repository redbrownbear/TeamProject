// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_GlideUnVisible.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_GlideUnVisible::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
	PlayerCharacter->GetWeaponManagerComponent()->GetGlider()->SetVisibility(false);
	
	
}