// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "ZeldaController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AZeldaController : public ANpcController
{
	GENERATED_BODY()
	
public:
	AZeldaController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
