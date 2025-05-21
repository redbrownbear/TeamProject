// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BokoblinAnimInstance.generated.h"

class UBokoblinFSMComponent;

UCLASS()
class TEAMPROJECT_API UBokoblinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UBokoblinAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<UBokoblinFSMComponent> FSMComponent = nullptr;

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
	bool bIsSignal = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSuspicious = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAimingBow = false;
};
