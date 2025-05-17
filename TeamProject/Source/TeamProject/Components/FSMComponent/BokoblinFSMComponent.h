// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/MonsterFSMComponent.h"
#include "BokoblinFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBokoblinFSMComponent : public UMonsterFSMComponent
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdatePatrol(float DeltaTime) override;
	virtual void UpdateSuspicious(float DeltaTime) override;
	virtual void UpdateAlert(float DeltaTime) override;
	virtual void UpdateCombat(float DeltaTime) override;
};
