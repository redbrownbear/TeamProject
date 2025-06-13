// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TempleSwitchManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlapTempleBall);
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTempleSwitchManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void NotifyOverlapTempleBall();
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnOverlapTempleBall OnOverlapTempleBall;
};
