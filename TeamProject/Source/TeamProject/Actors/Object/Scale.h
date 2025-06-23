// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scale.generated.h"

class UBoxComponent;
class AScaleGate;

UCLASS()
class TEAMPROJECT_API AScale : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScale();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Base;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Lever;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> LeftPlate;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> RightPlate;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> LeftWeightArea;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> RightWeightArea;

    UPROPERTY(EditAnywhere)
    TObjectPtr<AScaleGate> ScaleGate;

protected:
    UFUNCTION()
    void UpdateWeight();

    UFUNCTION()
    void ApplyOffsetFromWeight();

private:
    float LeftWeight = 0.f;
    float RightWeight = 0.f;

    FVector LeftStart;
    FVector RightStart;

};
