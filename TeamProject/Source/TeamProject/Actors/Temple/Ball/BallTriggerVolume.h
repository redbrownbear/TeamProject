// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "BallTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABallTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};
