// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_LynelTurnLeftEnd.h"

#include "Actors/Monster/Monster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_LynelTurnLeftEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
		{
			if (ULynelFSMComponent* LynelFSMComponent = Cast<ULynelFSMComponent>(FSMComponent))
			{
				LynelFSMComponent->SetTurnLeftCount(LynelFSMComponent->GetTurnLeftCount() + 1);
			}
		}
	}
}
