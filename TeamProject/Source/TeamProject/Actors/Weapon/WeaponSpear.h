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

};

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AWeaponSpear : public AWeaponBase
{
	GENERATED_BODY()
	
	
	AWeaponSpear();









private:
	UPROPERTY()
	USpearAttackDataAsset* DataAsset;

};
