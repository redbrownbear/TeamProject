// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelSheathWeapon.h"
#include "Actors/Monster/Monster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"
#include "Actors/Item/WorldWeapon.h"
#include "Misc/Utils.h"


void UAnimNotify_LynelSheathWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		if (ULynelFSMComponent* FSMComponent = Cast<ULynelFSMComponent>(Monster->GetFSMComponent()))
		{
			const AWorldWeapon* WW = FSMComponent->GetCurrentWeapon();

			const EWeaponKind eKind = WW->GetWorldWeaponKind();

			switch (eKind)
			{
			case EWeaponKind::SWORD:
			case EWeaponKind::SPEAR:
			case EWeaponKind::LSWORD:
				FSMComponent->DrawMeleeWeapon();
				break;
			case EWeaponKind::BOW:
				FSMComponent->DrawBowWeapon();
				break;
			default:
				break;
			}
		}
	}
}
