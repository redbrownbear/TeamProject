// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/TimeManager.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Actors/Monster/CharacterMonster.h"
void UTimeManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    // 필요 시 초기화
}

void UTimeManagerSubsystem::Deinitialize()
{
    // 필요 시 정리
}

void UTimeManagerSubsystem::Tick(float DeltaTime)
{
    if (bTimePaused)
    {
        CustomDeltaTime = DeltaTime;
    }
    else
    {
        CustomDeltaTime = DeltaTime * TimeScale;
    }
    if (bIsJust) 
    {
        mJustTime += DeltaTime;
        if (TIME_JUST <= mJustTime)
        {
            mJustTime = 0.f;
            bIsJust = false;
            SetTimeScale(TIMESCALE_NML);
        }
    }
}

float UTimeManagerSubsystem::GetCustomDeltaTime() const
{
    return CustomDeltaTime;
}

void UTimeManagerSubsystem::SetTimeScale(float NewScale)
{
    TimeScale = NewScale;
    OnTimeScaleUpdated.Broadcast(NewScale);

    //
    //if (!FMath::IsNearlyEqual(TimeScale, NewScale))
    //{
    //    TimeScale = NewScale;
    //    OnTimeScaleUpdated.Broadcast(NewScale);
    //}
    
}

float UTimeManagerSubsystem::GetTimeScale() const
{
    //return 1.f;
    return TimeScale;
}
