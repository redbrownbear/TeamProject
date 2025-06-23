// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Engine/DataAsset.h"
#include "WeaponSpear.generated.h"




UCLASS()
class TEAMPROJECT_API USpearAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, category = "Attack")
	TArray<UAnimMontage*> Attack_Normal;
	UPROPERTY(EditAnywhere, category = "Attack")
	TArray<UAnimMontage*> Attack_Advance;
	UPROPERTY(EditAnywhere, category = "Attack")
	TArray<UAnimMontage*> Attack_Just;
	UPROPERTY(EditAnywhere, category = "Equip")
	UAnimMontage* Equip_Montage;
	UPROPERTY(EditAnywhere, category = "Equip")
	UAnimMontage* UnEquip_Montage;
	
	UPROPERTY(EditAnywhere, category = "Move")
	UAnimMontage* Move_Upper;
};

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AWeaponSpear : public AWeaponBase
{
	GENERATED_BODY()
	
	
	AWeaponSpear();

	virtual void BeginPlay() override;

public:

	UAnimMontage* GetMoveUpperMontage() { return DataAsset->Move_Upper; }

	virtual void LeftClickAction();
	virtual void RightClickAction();

	virtual void Attack();

	void EmptyDamagedActors();

	void ComboReset() { mCombo = 0; }


private:
	UPROPERTY()
	USpearAttackDataAsset* DataAsset;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* CascadeTrailFX;

	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> HitEffectComponent;

	UPROPERTY()
	UParticleSystem* HitEffectFX;

	TArray<TObjectPtr<AActor>> DamagedActors;


	int32 mCombo = 0;
	int32 MaxCombo = 3;
};
