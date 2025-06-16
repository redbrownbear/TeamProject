// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMyProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
