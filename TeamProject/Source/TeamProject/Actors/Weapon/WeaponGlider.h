// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "WeaponGlider.generated.h"


class UNiagaraComponent;

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AWeaponGlider : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	AWeaponGlider();
	
	virtual void BeginPlay() override;

public:
	void SetVisibleGldier(bool bFlag);

	void StartTrailEffect();

	void EndTrailEffect();
private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> LNiagaraSystemComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> RNiagaraSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* NiagaraFX;
	
};
