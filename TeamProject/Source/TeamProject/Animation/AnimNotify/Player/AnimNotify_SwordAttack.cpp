// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_SwordAttack.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_SwordAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player_C)
	{

		AWeaponSword* Sword = Cast<AWeaponSword>(Player_C->GetWeapon()->GetChildActor());
		Sword->Attack();
		
	}
}
