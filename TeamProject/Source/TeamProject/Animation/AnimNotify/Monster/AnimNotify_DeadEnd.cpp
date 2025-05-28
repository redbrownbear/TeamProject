// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_DeadEnd.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Monster/CharacterMonster.h"

void UAnimNotify_DeadEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APawnMonster* PawnMonster = Cast<APawnMonster>(MeshComp->GetOwner()))
	{
		///PawnMonster->DeadEnd();
	}
	else if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		//CharacterMonster->DeadEnd();
	}

}
