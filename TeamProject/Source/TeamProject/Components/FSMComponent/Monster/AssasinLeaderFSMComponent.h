// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "AssasinLeaderFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UAssasinLeaderFSMComponent : public UMonsterFSMComponent
{
	GENERATED_BODY()
public:
	UAssasinLeaderFSMComponent();

protected:
	virtual void HandleState(float DeltaTime) override;

public:
	virtual void ChangeState(EMonsterState NewState) override;

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdatePatrol(float DeltaTime) override;
	virtual void UpdateAlert(float DeltaTime) override;
	virtual void UpdateSignal(float DeltaTime) override;

	virtual void UpdateCombat(float DeltaTime) override;
	virtual void UpdateSuspicious(float DeltaTime) override;

	virtual void UpdateHappy(float DeltaTime);

protected:
	float AttackCoolTime = 0.f;
	float AttackBigCoolTime = 0.f;
	bool bUp = true;
};
