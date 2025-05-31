// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/Utils.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

	
	EClimb_State GetClimbMode() { return Climb_State; }

	bool ClimbingLineTrace(FHitResult& HitResult);
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;

	
	void SetMovementClimb() { MovementMode=MOVE_Flying; bIsClimbing = true; }
	bool IsClimbing() { return bIsClimbing; }

	bool TrySetMoveClimb(FVector2D ActionValue);


	bool CanClimbUpLand();
	bool CanClimbDownLand();

	void SetClimbMode(bool _bool);
	void SetGlideMode(bool _bool);

	void GlidingMove(FVector2D ActionValue);

	UAnimMontage* GTEST() { return LandUpMontage; }


private:
	bool CanGlide();


private:
	


	UAnimMontage* LandUpMontage;
	UPROPERTY()
	EClimb_State Climb_State = EClimb_State::Climb;

public:
	bool bIsClimbing = false;
	bool bIsGliding = false;
};
