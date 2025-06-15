// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AssasinLeaderAnimInstance.generated.h"

class UAssasinLeaderFSMComponent;
class UTimeManagerSubsystem;

UCLASS()
class TEAMPROJECT_API UAssasinLeaderAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAssasinLeaderAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;	

protected:
	UPROPERTY()
	TObjectPtr<UAssasinLeaderFSMComponent> FSMComponent = nullptr;
	TObjectPtr<UTimeManagerSubsystem> TimeManager = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatIdle = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHappyWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSuspicious = false;
};
