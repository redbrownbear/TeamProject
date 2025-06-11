// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Effect/ParticleEffect.h"
#include "ParticleEffect_AlphaUpScaleUp.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AParticleEffect_AlphaUpScaleUp : public AParticleEffect
{
	GENERATED_BODY()
	
protected:
	virtual void Tick(float DeltaTime) override;
};
