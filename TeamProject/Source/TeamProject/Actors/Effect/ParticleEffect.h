// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleEffect.generated.h"

struct FParticleEffectTableRow;
UCLASS()
class TEAMPROJECT_API AParticleEffect : public AActor
{
	GENERATED_BODY()

public:
	AParticleEffect(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void SetData(const FName& ParticleEffectName);
	void SetParticleSystem(UParticleSystem* ParticleSystem);
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
	UPROPERTY(EditAnywhere, meta = (RowType = "ParticleEffectTableRow"))
	FDataTableRowHandle DataTableRowHandle;
	const UDataTable* ParticleEffectDataTable = nullptr;
	FParticleEffectTableRow* ParticleEffectTableRow = nullptr;

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneComponent;
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> ParticleEffectComponent;

public:
	void SetLifeTime(float _LifeTime) { fLifeTime = _LifeTime; }

protected:
	float fCurrentLifeTime;
	float fLifeTime;
};
