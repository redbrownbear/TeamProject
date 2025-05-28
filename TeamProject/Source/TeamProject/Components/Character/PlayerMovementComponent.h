// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerMovementComponent : public UCharacterMovementComponent
{

public:
	UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer);


	GENERATED_BODY()
	
	bool ClimbingLineTrace(FHitResult& HitResult);
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;

	void SetMovementClimb() { MovementMode=MOVE_Flying; bIsClimbing = true; }
	bool IsClimbing() { return bIsClimbing; }

	bool TrySetMoveClimb();


	void SetClimbMode(bool _bool);




private:
	bool bIsClimbing = false;
	


};
