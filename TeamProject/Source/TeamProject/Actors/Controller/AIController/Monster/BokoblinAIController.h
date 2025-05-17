// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "BokoblinAIController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABokoblinAIController : public AMonsterAIController
{
	GENERATED_BODY()
	
public:
	ABokoblinAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
