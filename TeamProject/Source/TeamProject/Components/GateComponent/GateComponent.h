// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UGateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void OpenGate();

	UFUNCTION()
	void OpenGateSequence();

protected:
	UFUNCTION()
	void MoveGateTick();

private:
	FTimerHandle GateMoveTimer;
	FVector StartLocation;
	FVector TargetLocation;

	UPROPERTY()
	float ElapsedTime = 0.f;

	UPROPERTY()
	float Duration = 1.0f;
		
};
