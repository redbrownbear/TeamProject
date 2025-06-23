// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "Misc/Utils.h"
#include "WeaponChildActorComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UWeaponChildActorComponent : public UChildActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	EWeapon_Type Weapon_Type;

public:
	
	void SetWeaponKind(EWeapon_Type InType) { Weapon_Type = InType; }
};
