// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimeManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeScaleUpdated, float, TimeScale);

class APawnMonster;
class ACharacterMonster;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTimeManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void Tick(float DeltaTime);
    float GetCustomDeltaTime() const;

    void SetTimeScale(float NewScale);  // 시간 배속 설정

public:

    UPROPERTY(BlueprintAssignable)
    FOnTimeScaleUpdated OnTimeScaleUpdated;

private:
    float TimeScale = 1.0f;
    bool bTimePaused = false;
    float CustomDeltaTime = 0.0f;

    float JustTime = 2.0f;
    float mJustTime = 0.f;

    bool bIsJust = false;


};
