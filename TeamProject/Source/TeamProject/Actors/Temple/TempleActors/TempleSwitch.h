// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempleSwitch.generated.h"

class USphereComponent;
class ALockedGate;
class ATempleActor;
class ATempleActorSpawner;

UCLASS()
class TEAMPROJECT_API ATempleSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATempleSwitch();

public:
	bool GetGateOpened() { return bIsOpenedGate; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void UnlockGate();

protected:
	UFUNCTION()
	void OnBeginOverlapWithBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATempleActor> TempleActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temple")
	TObjectPtr<ALockedGate> LockedGate;

private:
	bool bHasSpawnedOnce = false;
	bool bIsOpenedGate = false;
};
