// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_LynelTurnLeftEnd.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_LynelTurnLeftEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
		{
			if (ULynelFSMComponent* LynelFSMComponent = Cast<ULynelFSMComponent>(FSMComponent))
			{
				UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_LynelTurnRightEnd::Notify"));
				LynelFSMComponent->SetTurnLeftCount(LynelFSMComponent->GetTurnLeftCount() + 1);
			}
		}
	}
}
