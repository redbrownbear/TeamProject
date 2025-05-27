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
	

	UPROPERTY()
	int32 Health;
	UPROPERTY()
	int32 Damage;
	

public:
	UPlayerStatusComponent();

	
	
	
	
	






};
