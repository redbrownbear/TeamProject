// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Misc/Utils.h"
#include "HinoxFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UHinoxFSMComponent : public UMonsterFSMComponent
{
	GENERATED_BODY()
public:
	UHinoxFSMComponent();

protected:
	virtual void HandleState(float DeltaTime) override;
public:
	virtual void ChangeState(EMonsterState NewState) override;

protected:
	EHinoxCombatIndex eCombatIndex = EHinoxCombatIndex::Foot;
	float DamageEyeElapsedTime = 0.f;

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateAlert(float DeltaTime) override;
	virtual void UpdateCombat(float DeltaTime) override;
	virtual void UpdateDamageEye(float DeltaTime);
	virtual void UpdateDead(float DeltaTime);
	virtual void UpdateTemp(float DeltaTime);

};
