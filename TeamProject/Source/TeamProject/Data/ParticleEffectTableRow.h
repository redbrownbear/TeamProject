// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParticleEffectTableRow.generated.h"

USTRUCT()
struct TEAMPROJECT_API FParticleEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "ParticleEffect")
	TObjectPtr<UParticleSystem> EffectParticleSystem;

public:
	UPROPERTY(EditAnywhere, Category = "ParticleEffect|LifeTime")
	float LifeTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "ParticleEffect|LifeTime")
	bool bIsLifeTime = true;
};
