// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/FSMComponent/Npc/Scientist/ScientistFSMComponent.h"
#include "ScientistAnimInstance.generated.h"

class UScientistAnimInstance;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UScientistAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UScientistAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<UScientistFSMComponent> ScientistFSMComponent = nullptr;

	ENpcState eCurrentState;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSit = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStand = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsTalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHide = false;
};
