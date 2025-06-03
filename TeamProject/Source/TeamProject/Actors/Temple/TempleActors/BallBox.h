// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBox.generated.h"

class USphereComponent;
class ATempleSwitch;
class ATempleActor;
class ATempleActorSpawner;

UCLASS()
class TEAMPROJECT_API ABallBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckAndSpawnTempleActor();

public:
	//virtual void Tick(float DeltaSeconds) override;:
	/*UFUNCTION()
	void OnBeginOverlapWithBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);*/

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATempleActor> TempleActor = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATempleSwitch> TempleSwitch = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temple")
	TObjectPtr<ATempleActorSpawner> TempleActorSpawner;

private:
	FTimerHandle SpawnDelayTimer;

	bool bHasSpawnedOnce = false;
};
