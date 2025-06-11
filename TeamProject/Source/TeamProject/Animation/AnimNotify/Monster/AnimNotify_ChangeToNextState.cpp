// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_ChangeToNextState.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_ChangeToNextState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (ULynelFSMComponent* FSMComponent = Cast<ULynelFSMComponent>(Monster->GetFSMComponent()))
		{
			const EMonsterState eNextState = FSMComponent->GetNextState();
			const EMonsterState eCurrentState = FSMComponent->GetCurrentState();

			if (eNextState != eCurrentState)
			{
				FSMComponent->ChangeState(FSMComponent->GetNextState());
			}
		}
	}
}
