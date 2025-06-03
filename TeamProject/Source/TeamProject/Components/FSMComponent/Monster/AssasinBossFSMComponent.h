// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Misc/Utils.h"
#include "AssasinBossFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UAssasinBossFSMComponent : public UMonsterFSMComponent
{
	GENERATED_BODY()
public:
	UAssasinBossFSMComponent();

protected:
	virtual void HandleState(float DeltaTime) override;

public:
	virtual void ChangeState(EMonsterState NewState) override;

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateCombat(float DeltaTime) override;
	virtual void UpdateStun(float DeltaTime);
	virtual void UpdateBarrier(float DeltaTime);
	virtual void UpdateStone(float DeltaTime);
	virtual void UpdateDamage(float DeltaTime);

protected:
	EAssasinBossPhase ePhase = EAssasinBossPhase::PHASE_1;
	EAssasinBossCombat eCombat = EAssasinBossCombat::BARRIER;

protected:
	void EnableFlyingMode(bool bFlag);
	void Hovering(float DeltaTime);

public:
	EAssasinBossPhase GetPhase() const { return ePhase; }
	void SetPhase(EAssasinBossPhase InPhase) { ePhase = InPhase; }



protected:
	float ToNextElapsedTime = 0.f;
	float BarrierElapsedTime = 0.f;
	float StoneElapsedTime = 0.f;
	float StunElapsedTime = 0.f;
	float DamageElapsedTime = 0.f;

};
