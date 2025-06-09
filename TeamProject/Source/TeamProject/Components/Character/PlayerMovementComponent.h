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
	UAnimMontage* GetGlideUnEquipMontage() { return GlideUnEquip; }


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

	void SetMoveState(EMove_State _State) { Move_State = _State; }
	EMove_State GetMoveState() { return Move_State; }

	UAnimMontage* GTEST() { return LandUpMontage; }

	void Hited();


private:
	bool CanGlide();


private:

	UPROPERTY()
	UAnimMontage* GlideUnEquip;
	UPROPERTY()
	UAnimMontage* HitMontage;
	UPROPERTY()
	UAnimMontage* LandUpMontage;
	UPROPERTY()
	EClimb_State Climb_State = EClimb_State::Climb;
	UPROPERTY()
	EMove_State Move_State = EMove_State::Run;

public:


	bool bIsClimbing = false;
	bool bIsGliding = false;
};
