// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Misc/Utils.h"
#include "PlayerStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerStatusComponent : public UStatusComponent
{
	GENERATED_BODY()
	

	UPROPERTY()
	int32 Health;
	UPROPERTY()
	int32 Damage;
	UPROPERTY()
	EWeapon_Type Next_Weapon;

public:
	UPlayerStatusComponent();
	void Equip_Sword();
	void Equip_Shield();
	void Equip_Bow();
	void UnEquip_Sword();
	void UnEquip_Shield();
	void UnEquip_Bow();
	
	
	
	EWeapon_Type GetNextWeaponType() { return Next_Weapon; }
	void SetNextWeaponType(EWeapon_Type _Type) { Next_Weapon = _Type; }






};
