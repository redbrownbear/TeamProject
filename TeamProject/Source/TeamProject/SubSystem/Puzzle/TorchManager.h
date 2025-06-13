// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TorchManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTorchesLit);

class ATorchStand;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTorchManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void RegisterTorch(ATorchStand* Torch);
    UFUNCTION()
    void NotifyTorchLit(ATorchStand* Torch);

    UPROPERTY(BlueprintAssignable)
    FOnAllTorchesLit OnAllTorchesLit;

private:
    TArray<TObjectPtr<ATorchStand>> TorchList;
    TSet<TObjectPtr<ATorchStand>> LitTorchSet;
};
