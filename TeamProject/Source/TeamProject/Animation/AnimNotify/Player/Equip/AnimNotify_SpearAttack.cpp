// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/Equip/AnimNotify_SpearAttack.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Weapon/WeaponSpear.h"

void UAnimNotify_SpearAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player_C)
	{

		AWeaponSpear* Spear = Cast<AWeaponSpear>(Player_C->GetWeapon()->GetChildActor());
		Spear->Attack();

	}
}
