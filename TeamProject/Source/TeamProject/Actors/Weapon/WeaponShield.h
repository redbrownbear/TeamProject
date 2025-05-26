// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "WeaponShield.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AWeaponShield : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponShield();

public:
	virtual void LeftClickAction() override;
	virtual void RightClickAction() override;

private:


	UPROPERTY()
	UAnimMontage* Wait_MTG;
	UPROPERTY()
	UAnimMontage* Just_MTG;

};
