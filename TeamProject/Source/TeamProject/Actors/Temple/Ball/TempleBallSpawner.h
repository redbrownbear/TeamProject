// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempleBallSpawner.generated.h"

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

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> BallClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FVector SpawnAreaMin;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FVector SpawnAreaMax;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval = 2.0f;

private:
    void SpawnBall();
    FTimerHandle SpawnTimerHandle;

};
