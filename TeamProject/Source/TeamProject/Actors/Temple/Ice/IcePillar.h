// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IcePillar.generated.h"

class UBoxComponent;
class UTimelineComponent;
class UNiagaraSystem;
class UMaterialInterface;

UCLASS()
class TEAMPROJECT_API AIcePillar : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIcePillar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 파괴 처리
	UFUNCTION()
	void DestroyPillar();

	void SetRiseDirection(FVector InDirection);
	void SetPivotLocation(FVector InPosition);
	void SetCanSpawn(bool bFlag) { bCanSpawn = bFlag; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	float MaxSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	float MaxHeight = 300.0f; // 매크로로 바꿀까

	/*UPROPERTY(EditDefaultsOnly, Category = "Cryonis")
	TObjectPtr<UNiagaraSystem> BreakEffect;*/

protected:
	UMaterialInterface* MaterialInterface;
	UMaterialInstanceDynamic* DynamicMaterialInstance;

private:
	FVector StartLocation;
	float CurrentRise = 0.f;
	bool bIsRising = true;
	bool bCanSpawn = false;

	FVector RiseDirection;
	FVector PivotLocation;
};
