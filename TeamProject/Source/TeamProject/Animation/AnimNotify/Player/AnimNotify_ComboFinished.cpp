// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_ComboFinished.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Weapon/WeaponSpear.h"
#include "Actors/Weapon/WeaponSword.h"
void UAnimNotify_ComboFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		if (AWeaponBase* WeaponBase = Cast<AWeaponBase>(Player_C->GetWeapon()->GetChildActor()))
		{
			if (AWeaponSword* Sword = Cast<AWeaponSword>(WeaponBase))
			{
				Sword->ComboReset();
			}
			else if (AWeaponSpear* Spear = Cast<AWeaponSpear>(WeaponBase))
			{
				Spear->ComboReset();
			}
		}
	}
}
