// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_EquipWeapon.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();

	EWeapon_Type NextWeapon = WeaponManagerComponent->GetNextWeaponType();

	if (NextWeapon == EWeapon_Type::Sword)
	{
		PlayerCharacter->GetSword()->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale , TEXT("Weapon_R"));
		EEquip_State m_State = WeaponManagerComponent->GetEquipState();
		if (m_State == EEquip_State::None)
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::Sword);
		}
		else
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::Sword_Shield);
		}
	}
	else if (NextWeapon == EWeapon_Type::Bow)
	{
		PlayerCharacter->GetBow()->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_L"));
		WeaponManagerComponent->SetEquipState(EEquip_State::Bow);
	}
	else if (NextWeapon == EWeapon_Type::Shield)
	{
		PlayerCharacter->GetShield()->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_L"));
		EEquip_State m_State = WeaponManagerComponent->GetEquipState();

		if (m_State == EEquip_State::None)
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::Shield);
		}
		else
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::Sword_Shield);
		}
	}

}
