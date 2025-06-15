// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AssasinBossAnimInstance.generated.h"

class UAssasinBossFSMComponent;
class UTimeManagerSubsystem;
UCLASS()
class TEAMPROJECT_API UAssasinBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAssasinBossAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<UAssasinBossFSMComponent> FSMComponent = nullptr;
	TObjectPtr<UTimeManagerSubsystem> TimeManager = nullptr;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFlyIdle = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStun = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStoneFirst = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStoneSecond = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsBarrier = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDamage = false;
};
