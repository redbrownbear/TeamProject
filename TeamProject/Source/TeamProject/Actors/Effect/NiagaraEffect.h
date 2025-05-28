// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraEffect.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
struct FNiagaraEffectTableRow;
UCLASS()
class TEAMPROJECT_API ANiagaraEffect : public AActor
{
	GENERATED_BODY()

public:
	ANiagaraEffect(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform);
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle DataTableRowHandle;
	TObjectPtr<FNiagaraEffectTableRow> NiagaraEffectData;

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneComponent;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraEffectComponent;

public:
	void SetNiagaraSystem(UNiagaraSystem* InAsset);
	void SetLifeTime(float _LifeTime);

protected:
	float fCurrentLifeTime = 0.f;
	float fLifeTime = 0.f;
};
