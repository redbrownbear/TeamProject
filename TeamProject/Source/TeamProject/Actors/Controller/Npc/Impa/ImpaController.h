// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "ImpaController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AImpaController : public ANpcController
{
	GENERATED_BODY()
	
public:
	AImpaController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
