// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "MusicianController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMusicianController : public ANpcController
{
	GENERATED_BODY()
	
public:
	AMusicianController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
