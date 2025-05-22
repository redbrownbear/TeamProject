// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
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

public:
	void SetData(FDataTableRowHandle InDataTableRowHandle);
};
