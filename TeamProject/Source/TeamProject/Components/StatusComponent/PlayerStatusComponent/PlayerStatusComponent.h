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
	
private:
	UPROPERTY()
	int32 Damage = 1;
	

public:
	UPlayerStatusComponent();

public:
	int32 GetDamage() const { return Damage; }
	void SetDamage(int32 InDamage) { Damage = InDamage; }
};
