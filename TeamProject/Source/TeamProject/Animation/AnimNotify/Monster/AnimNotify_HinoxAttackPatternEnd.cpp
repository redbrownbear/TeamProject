// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxAttackPatternEnd.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"

void UAnimNotify_HinoxAttackPatternEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UHinoxFSMComponent* FSM = Cast<UHinoxFSMComponent>(Monster->GetFSMComponent()))
		{
			FSM->UpdateCombatIndex();
		}
	}
}
