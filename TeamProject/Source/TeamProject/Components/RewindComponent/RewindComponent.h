// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RewindComponent.generated.h"


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

private:
	TArray<FVector> PositionHistory;
	float RecordDuration = 5.0f; // 되감기 가능 시간 
	float TimeStep = 0.05f; // 1초에 20프레임 저장
	float TimeAccumulator = 0.0f;

	bool bIsRewinding = false;
	int32 CurrentRewindIndex = 0;
		
};
