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
	
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugSphere);

	void TraceFromEyeHeight(float TraceDistance, float TraceStartOffset);

	void DoLineTraceSingleByObject(FVector Start, FVector End, bool bShowDebugLine);


public:

	UPROPERTY(EditAnywhere, category = "Climb")
	float ClimbCapsuleTraceRadius;
	UPROPERTY(EditAnywhere, category = "Climb")
	float ClimbCapsuleTraceHalfHeight;
	UPROPERTY(EditAnywhere, category = "Climb")
	TArray<TEnumAsByte<EObjectTypeQuery>> ClimbableSurfaceTraceTypes;


};
