// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "ArtistController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AArtistController : public ANpcController
{
	GENERATED_BODY()
	
public:
	AArtistController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
