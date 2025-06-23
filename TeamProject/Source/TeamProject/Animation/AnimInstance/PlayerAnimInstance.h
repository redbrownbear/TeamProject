// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Misc/Utils.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
	void SetPitch();
	UFUNCTION()
	void SetPlayRate(float _Rate) { PlayRate = _Rate; }
	UFUNCTION()
	void SetEquipState(EEquip_State _State);

public:

	UPROPERTY()
	class UPawnMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	double Speed = 0.0;

	UPROPERTY(BlueprintReadOnly)
	bool bShoudMove = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCrouch = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsBackFlip = false;

	UPROPERTY(BlueprintReadOnly)
	FVector2D ActionValue;

	UPROPERTY(BlueprintReadOnly)
	bool bIsZoom = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWaitShield = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCliming = false;
	UPROPERTY(BlueprintReadOnly)
	FRotator Rotator;
	UPROPERTY(BlueprintReadOnly)
	bool bIsClimingLand = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsGliding = false;
	UPROPERTY(BlueprintReadOnly)
	float PlayRate = 1.f;
	UPROPERTY(BlueprintReadOnly)
	EEquip_State Equip_State = EEquip_State::None;
};
