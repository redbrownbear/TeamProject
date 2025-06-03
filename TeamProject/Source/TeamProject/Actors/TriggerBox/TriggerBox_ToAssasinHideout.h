// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerBox_ToAssasinHideout.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ATriggerBox_ToAssasinHideout : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATriggerBox_ToAssasinHideout();

protected:
	UFUNCTION()
	void OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
