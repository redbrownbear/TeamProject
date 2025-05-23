// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Misc/Utils.h"
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
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY()
	TObjectPtr<UMonsterFSMComponent> MonsterFSMComponent;

public:
	UFUNCTION()
	void SetAIEnabled(bool bEnabled);

public:
	virtual uint8 GetGenericTeamId() { return FGenericTeamId(MONSTER_GENERIC_TEAM_ID); }
};
