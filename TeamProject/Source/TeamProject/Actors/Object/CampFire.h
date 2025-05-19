// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CampFire.generated.h"

class UNiagaraComponent;

UCLASS()
class TEAMPROJECT_API ACampFire : public AActor
{
	GENERATED_BODY()
	
public:	
	ACampFire();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

};
