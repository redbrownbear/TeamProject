// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Actors/Projectile/Arrow/Projectile_Arrow.h"
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

	virtual void BeginPlay() override;

	virtual void LeftClickAction() override;
	virtual void RightClickAction() override;
	UAnimMontage* GetChargingMTG() { return ChargingMTG; }

	void FireArrow();

	void SetArrowFire(bool _bool);

	void SetArrowVisibility(bool _bool) { ChargedArrow->SetStaticMeshVisibility(_bool); }
	
	void SetNiagaraSystemAssetNone() { ChargedArrow->SetNiagaraSystemAssetNone(); }

private:


	bool bIsFire;

	UPROPERTY()
	UAnimMontage* ChargingMTG;
	UPROPERTY()
	UAnimMontage* ShootMTG;
	UPROPERTY()
	AProjectile_Arrow* ChargedArrow;




	TSubclassOf<class AProjectile_Arrow> ArrowClass;

	

	FName SocketName = "Arrow_Socket";
};
