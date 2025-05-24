// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_ChangeToNextState.h"
#include "Actors/Monster/Monster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_ChangeToNextState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		if (ULynelFSMComponent* FSMComponent = Cast<ULynelFSMComponent>(Monster->GetFSMComponent()))
		{
			const EMonsterState eNextState = FSMComponent->GetNextState();
			const EMonsterState eCurrentState = FSMComponent->GetMonsterState();

			if (eNextState != eCurrentState)
			{
				FSMComponent->ChangeState(FSMComponent->GetNextState());
			}
		}
	}
}
