// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HinoxAnimInstance.generated.h"

class UHinoxFSMComponent;

UCLASS()
class TEAMPROJECT_API UHinoxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHinoxAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<UHinoxFSMComponent> FSMComponent = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDamageEye = false;
};
