// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelDrawBow.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void UAnimNotify_LynelDrawBow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (ULynelFSMComponent* FSMComponent = Cast<ULynelFSMComponent>(CharacterMonster->GetFSMComponent()))
		{
			FSMComponent->DrawBowWeapon();
		}
	}
	else if (APawnMonster* PawnMonster = Cast<APawnMonster>(MeshComp->GetOwner()))
	{
		if (UMonsterFSMComponent* FSMComponent = Cast<ULynelFSMComponent>(PawnMonster->GetFSMComponent()))
		{
			FSMComponent->DrawBowWeapon();
		}
	}
}
