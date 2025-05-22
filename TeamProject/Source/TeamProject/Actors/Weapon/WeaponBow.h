// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "WeaponBow.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AWeaponBow : public AWeaponBase
{
	GENERATED_BODY()
	





public:
	AWeaponBow();

	virtual void LeftClickAction() override;
	virtual void RightClickAction() override;


private:


	UPROPERTY()
	UAnimMontage* Attack_MTG;

	bool bCanAttack = true;
};
