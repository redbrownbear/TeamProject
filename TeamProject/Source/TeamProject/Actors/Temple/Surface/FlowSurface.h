// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowSurface.generated.h"

#define FLOWSURFACE_FLOOR_NUM				20
//#define FLOWSURFACE_DEFAULT_BOX_EXTENT		FVector(128.0, 540.f, 1.0)
#define FLOWSURFACE_MOVING_SPEED			100.f
#define FLOATINGACTOR_FORCE					800.f

class UBoxComponent;
class USplineComponent;

UCLASS()
class TEAMPROJECT_API AFlowSurface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlowSurface();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

    USplineComponent* GetSplineComponent() { return SplineComponent; }

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> DefaultSceneRoot;

    UPROPERTY(VisibleAnywhere)
    TArray<TObjectPtr<UBoxComponent>> CollisionComponent_Array;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USplineComponent> SplineComponent;

    UPROPERTY(EditAnywhere, Category = "Flow Surface")
    FVector DefaultBoxExtent = FVector(128.f, 540.f, 1.f);

    TArray<float> fDistanceAlongSpline_Array;

protected:
    UPROPERTY(EditAnywhere)
    float FlowSpeed = 0.f;
    
    bool bMove = false;

protected:
    UPhysicalMaterial* PhysicalMaterial = nullptr;
};
