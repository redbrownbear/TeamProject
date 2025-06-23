// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "WeaponSword.generated.h"

class UParticleSystemComponent;
class UParticleSystem;

/**
 * 
 */
class UNiagaraComponent;
UCLASS()
class TEAMPROJECT_API AWeaponSword : public AWeaponBase
{
	GENERATED_BODY()






public:
	AWeaponSword();

	virtual void BeginPlay() override;


public:

	virtual void LeftClickAction() override;

	void SetCanAttack();

	void SetCanMove();

	void Attack();
	void EmptyDamagedActors();

	void StartTrailEffect();
	void StopTrailEffect();

	void StartHitEffect();
	void StopHitEffect();

	void ComboReset() { CurrentComboIndex = 0; }
private:


	UPROPERTY()
	TArray<UAnimMontage*> Arr_Sword_Attack_MTG;

	UPROPERTY()
	int32 CurrentComboIndex = 0;

	UPROPERTY()
	int32 PrevComboIndex = 0;

	UPROPERTY()
	int32 MaxComboIndex = 0;

	bool bCanAttack= true;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* CascadeTrailFX;

	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> HitEffectComponent;

	UPROPERTY()
	UParticleSystem* HitEffectFX;

	TArray<TObjectPtr<AActor>> DamagedActors;

	
};
