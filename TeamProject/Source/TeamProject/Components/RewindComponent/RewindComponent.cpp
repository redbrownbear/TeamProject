// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RewindComponent/RewindComponent.h"
#include "RewindComponent.h"

// Sets default values for this component's properties
URewindComponent::URewindComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void URewindComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void URewindComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Owner = GetOwner();
    if (!Owner) return;

    if (bIsRewinding)
    {
        if (CurrentRewindIndex >= 0)
        {
            Owner->SetActorLocation(PositionHistory[CurrentRewindIndex]);
            --CurrentRewindIndex;
        }
        else
        {
            StopRewind(); // 자동 종료
        }
    }
    else
    {
        TimeAccumulator += DeltaTime;
        if (TimeAccumulator >= TimeStep)
        {
            TimeAccumulator = 0.0f;
            PositionHistory.Insert(Owner->GetActorLocation(), 0); // 최신 위치 맨 앞에 저장

            // 오래된 기록 삭제
            float MaxRecord = RecordDuration / TimeStep;
            if (PositionHistory.Num() > MaxRecord)
            {
                PositionHistory.RemoveAt(PositionHistory.Num() - 1);
            }
        }
    }
}

void URewindComponent::StartRewind()
{
	bIsRewinding = true;
	CurrentRewindIndex = PositionHistory.Num() - 1;
}

void URewindComponent::StopRewind()
{
	bIsRewinding = false;
}

