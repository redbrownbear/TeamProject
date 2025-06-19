// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterSurface.generated.h"

class UBoxComponent;
class UWaterComponent;

UCLASS()
class TEAMPROJECT_API AWaterSurface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterSurface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWaterComponent> WaterComponent;
};
