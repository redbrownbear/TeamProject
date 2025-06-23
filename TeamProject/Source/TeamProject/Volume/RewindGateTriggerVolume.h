// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "RewindGateTriggerVolume.generated.h"

class ARewindGate;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ARewindGateTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
	
public:
    ARewindGateTriggerVolume();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

public:
    UPROPERTY(EditAnywhere)
    TObjectPtr<ARewindGate> TargetGate;

};
