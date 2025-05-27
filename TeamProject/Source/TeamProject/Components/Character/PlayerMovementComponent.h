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
	
	void ClimbingLineTrace();
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;


	


};
