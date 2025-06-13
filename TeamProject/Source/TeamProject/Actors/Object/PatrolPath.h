// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class TEAMPROJECT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	USplineComponent* GetPath() { return SplineComponent; }

protected:
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere)
	FGuid PatrolPathGuid;

public:
	FVector GetSplinePointLocation(int32 InIndex);
	int32 GetSplineMaxIndex() const;
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	FGuid GetPatrolPathGuid() const { return PatrolPathGuid; }
};
