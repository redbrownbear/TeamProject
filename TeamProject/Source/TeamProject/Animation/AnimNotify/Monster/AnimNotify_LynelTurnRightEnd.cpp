// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelTurnRightEnd.h"
#include "Actors/Monster/Monster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_LynelTurnRightEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
		{
			if (ULynelFSMComponent* LynelFSMComponent = Cast<ULynelFSMComponent>(FSMComponent))
			{
				LynelFSMComponent->SetTurnRightCount(LynelFSMComponent->GetTurnRightCount() + 1);
			}
		}
	}
}
