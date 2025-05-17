// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "MoriblinAIController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMoriblinAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	AMoriblinAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
