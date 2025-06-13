// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EnterHouse.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AEnterHouse : public ATriggerBox
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
