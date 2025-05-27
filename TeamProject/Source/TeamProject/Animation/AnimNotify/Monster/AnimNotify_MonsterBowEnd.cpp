// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_MonsterBowEnd.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"

void UAnimNotify_MonsterBowEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
		{
			FSMComponent->ChangeState(EMonsterState::Combat);
		}
	}
}
