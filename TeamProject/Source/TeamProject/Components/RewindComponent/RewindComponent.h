// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RewindComponent.generated.h"

#define REWIND_TRANSFORM_COUNT_MAX				400

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API URewindComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URewindComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartRewind();
	void StopRewind();

	bool IsRewinding() const { return bIsRewinding; }

protected:
	void StoreRewind();
	void ApplyRewind();


private:
	TArray<FTransform> TransformHistory;
	bool bIsRewinding = false;
	int32 CurrentIndex = 0;
	int32 RewindIndex = -1;
	bool bBufferFilled = false;

public:
	void SetColorNormal();
	void SetColorScanned();
};
