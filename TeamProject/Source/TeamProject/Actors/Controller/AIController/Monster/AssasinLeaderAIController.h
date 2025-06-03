// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "AssasinLeaderAIController.generated.h"

UCLASS()
class TEAMPROJECT_API AAssasinLeaderAIController : public AMonsterAIController
{
	GENERATED_BODY()
public:
	AAssasinLeaderAIController();
	virtual void Tick(float DeltaTime) override;

protected:	
	virtual void BeginPlay() override;
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

};
