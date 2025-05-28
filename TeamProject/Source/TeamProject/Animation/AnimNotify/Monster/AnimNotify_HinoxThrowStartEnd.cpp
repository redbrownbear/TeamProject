// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxThrowStartEnd.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"
void UAnimNotify_HinoxThrowStartEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
	{
		return;
	}
	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UHinoxFSMComponent* FSMComponent = Cast<UHinoxFSMComponent>(Monster->GetFSMComponent()))
		{
			Monster->PlayMontage(EMonsterMontage::THROW_STONE_END);
		}
	}
}
