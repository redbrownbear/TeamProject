// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ExitHouse.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AExitHouse : public ATriggerBox
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
