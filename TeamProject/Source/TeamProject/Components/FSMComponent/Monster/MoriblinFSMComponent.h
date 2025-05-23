// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "MoriblinFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMoriblinFSMComponent : public UMonsterFSMComponent
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdatePatrol(float DeltaTime) override;
	virtual void UpdateSuspicious(float DeltaTime) override;
	virtual void UpdateAlert(float DeltaTime) override;
	virtual void UpdateCombat(float DeltaTime) override;
};
