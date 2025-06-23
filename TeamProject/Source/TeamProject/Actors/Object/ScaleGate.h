// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScaleGate.generated.h"

UCLASS()
class TEAMPROJECT_API AScaleGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScaleGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	void OpenScaleGate();
	bool GetOpenGate() const { return bOpenGate; }
protected:
	void MoveGateTick(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

private:
	FTimerHandle GateMoveTimer;
	FVector StartLocation;
	FVector TargetLocation;


	UPROPERTY()
	float Duration = 1.0f;

	bool bOpenGate = false;

};
