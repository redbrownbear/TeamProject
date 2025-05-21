// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotifyUnEquipWeapon.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotifyUnEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	if (!WeaponManagerComponent)
	{
		return;
	}
	
	EWeapon_Type UnEquipWeapon = WeaponManagerComponent->GetUnEquipWeaponType();

	if (UnEquipWeapon == EWeapon_Type::Sword)
	{
		PlayerCharacter->GetSword()->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Socket"));
		EEquip_State m_State = WeaponManagerComponent->GetEquipState();
		if (m_State == EEquip_State::Sword)
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::None);
		}
		else
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::Shield);
		}
	}
	else if (UnEquipWeapon == EWeapon_Type::Bow)
	{
		PlayerCharacter->GetBow()->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Bow_Socket"));
		WeaponManagerComponent->SetEquipState(EEquip_State::None);
	}
	else if (UnEquipWeapon == EWeapon_Type::Shield)
	{
		PlayerCharacter->GetShield()->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Shield_Socket"));
		EEquip_State m_State = WeaponManagerComponent->GetEquipState();

		if (m_State == EEquip_State::Shield)
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::None);
		}
		else
		{
			WeaponManagerComponent->SetEquipState(EEquip_State::Sword);
		}
	}
}
