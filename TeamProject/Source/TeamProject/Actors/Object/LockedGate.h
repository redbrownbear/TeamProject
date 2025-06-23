// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockedGate.generated.h"

class UGateComponent;

UCLASS()
class TEAMPROJECT_API ALockedGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockedGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OpenGateSequence();

protected:
	void OpenGate();

	UFUNCTION()
	void ClearDungeon();

	UFUNCTION()
	void MoveGateTick();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGateComponent> GateComponent;

private:
	FTimerHandle GateMoveTimer;
	FVector StartLocation;
	FVector TargetLocation;

	UPROPERTY()
	float ElapsedTime = 0.f;

	UPROPERTY()
	float Duration = 1.0f;

private:
	int LittedTorchNum = 0;

public:
	void TorchLitOn();
};
