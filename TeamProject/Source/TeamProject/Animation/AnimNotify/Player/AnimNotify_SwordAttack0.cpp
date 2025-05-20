// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_SwordAttack0.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/AttackComponent/WeaponComponent.h"

void UAnimNotify_SwordAttack0::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player_C)
	{
		Player_C->GetSword()->SetCanAttack();
	}
}
