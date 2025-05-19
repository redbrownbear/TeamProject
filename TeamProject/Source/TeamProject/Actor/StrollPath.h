// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StrollPath.generated.h"

class USplineComponent;

UCLASS()
class TEAMPROJECT_API AStrollPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrollPath();

	USplineComponent* GetPath() { return SplineComponent; }

protected:
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComponent;

public:
	FVector GetSplinePointLocation(int32 InIndex);
	int32 GetSplineMaxIndex() const;

};
