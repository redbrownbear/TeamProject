// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_SwordCanCombo.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_SwordCanCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player_C)
	{
		Cast<AWeaponSword>(Player_C->GetSword()->GetChildActor())->SetCanAttack();
	}
}
