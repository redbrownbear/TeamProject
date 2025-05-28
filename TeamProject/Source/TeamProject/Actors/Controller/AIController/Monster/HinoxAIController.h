// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "HinoxAIController.generated.h"

UCLASS()
class TEAMPROJECT_API AHinoxAIController : public AMonsterAIController
{
	GENERATED_BODY()
public:
	AHinoxAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
