// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "HeirController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AHeirController : public ANpcController
{
	GENERATED_BODY()
	
public:
	AHeirController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
