// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempleBallSpawner.generated.h"

//class UTempleBallSpawnerComponent;
class ATempleBall;

UCLASS()
class TEAMPROJECT_API ATempleBallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATempleBallSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SpawnBall();

protected:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ATempleBall> BallClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnAreaExtent;

private:
	FTimerHandle SpawnTimer;

};
