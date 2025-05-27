// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelPatternEnd.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_LynelPatternEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
		{
			if (ULynelFSMComponent* LynelFSMComponent = Cast<ULynelFSMComponent>(FSMComponent))
			{
				LynelFSMComponent->ChangeState(EMonsterState::Combat);
			}
		}
	}
}
