// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class TEAMPROJECT_API ABarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	ABarrier();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystemComponent> ParticleEffectComponent;
protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float LifeTime = 0.f;

};
