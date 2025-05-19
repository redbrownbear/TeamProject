// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "StoreController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AStoreController : public ANpcController
{
	GENERATED_BODY()

public:
	AStoreController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
};
