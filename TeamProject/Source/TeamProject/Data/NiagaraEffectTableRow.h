// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "NiagaraSystem.h"
#include "NiagaraEffectTableRow.generated.h"

USTRUCT()
struct TEAMPROJECT_API FNiagaraEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "NiagaraEffect")
	TObjectPtr<UNiagaraSystem> EffectNiagaraSystem;

public:
	UPROPERTY(EditAnywhere, Category = "NiagaraEffect|LifeTime")
	float LifeTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "NiagaraEffect|LifeTime")
	bool bIsLifeTime = true;

public:
	UPROPERTY(EditAnywhere, Category = "NiagaraEffect|Transform")
	FTransform Transform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.f));
};
