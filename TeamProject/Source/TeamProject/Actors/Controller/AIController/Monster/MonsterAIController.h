// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

class UMonsterFSMComponent;

UCLASS()
class TEAMPROJECT_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY()
	TObjectPtr<UMonsterFSMComponent> MonsterFSMComponent;
};
