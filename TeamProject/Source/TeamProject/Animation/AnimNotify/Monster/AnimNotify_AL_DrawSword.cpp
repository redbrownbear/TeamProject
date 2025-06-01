// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_AL_DrawSword.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/AssasinLeaderFSMComponent.h"

void UAnimNotify_AL_DrawSword::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UAssasinLeaderFSMComponent* FSMComponent = Cast<UAssasinLeaderFSMComponent>(Monster->GetFSMComponent()))
		{
			FSMComponent->DrawMeleeWeapon();
		}
	}
}
