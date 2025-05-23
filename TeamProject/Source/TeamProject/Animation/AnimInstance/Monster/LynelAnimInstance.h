// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LynelAnimInstance.generated.h"

class ULynelFSMComponent;

UCLASS()
class TEAMPROJECT_API ULynelAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	ULynelAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<ULynelFSMComponent> FSMComponent = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSearch = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFire = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsEat = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDance = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSuspicious = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAimingBow = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAimingBowUpper = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAttackRunning = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAttackHorn = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAttackDash = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRodeo = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStun = false;

};
