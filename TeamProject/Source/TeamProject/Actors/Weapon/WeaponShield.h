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
	void SetGuard();

	virtual void LeftClickAction() override;
	virtual void RightClickAction() override;

	UAnimMontage* GetWaitMTG() { return Wait_MTG; }
	UAnimMontage* GetJustMTG() { return Just_MTG; }
private:


	UPROPERTY()
	UAnimMontage* Wait_MTG;
	UPROPERTY()
	UAnimMontage* Just_MTG;

};
