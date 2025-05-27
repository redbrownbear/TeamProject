// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IcePillar.generated.h"

class UBoxComponent;
class UTimelineComponent;
class UNiagaraSystem;

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

protected:
	// 얼음 기둥 상승 처리
	UFUNCTION()
	void AnimateRise(float Value);

	// 파괴 처리
	UFUNCTION()
	void DestroyPillar(); 

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Cryonis")
	TObjectPtr<UCurveFloat> RiseCurve;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> RiseTimeline;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	float MaxHeight = 300.0f; // 매크로로 바꿀까

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	float LifeTime = 10.0f; // 매크로로 바꿀까

	UPROPERTY(EditDefaultsOnly, Category = "Cryonis")
	TObjectPtr<UNiagaraSystem> BreakEffect;
};
