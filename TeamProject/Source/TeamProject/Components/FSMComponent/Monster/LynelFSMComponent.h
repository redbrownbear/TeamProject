// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Misc/Utils.h"
#include "LynelFSMComponent.generated.h"

UCLASS()
class TEAMPROJECT_API ULynelFSMComponent : public UMonsterFSMComponent
{
	GENERATED_BODY()
	
public:
	ULynelFSMComponent();

protected:
	virtual void HandleState(float DeltaTime) override;
public:
	virtual void ChangeState(EMonsterState NewState) override;

protected:
	ECombatIndex eCombatIndex = ECombatIndex::AimingBow;
	EMonsterState eNextState = EMonsterState::End;
	EReadyToAttackStep eReadyToAttackStep = EReadyToAttackStep::RunToLink;
	int32 LyenlTurnRightCount = 0;
	int32 LyenlTurnLeftCount = 0;
	float StunElapsedTime = 0.f;
	float RodeoElapsedTime = 0.f;
	int32 FireAttackTimes = 0;
	bool bHornAttackPassed = false;

public:
	EMonsterState GetNextState() const { return eNextState; }
	void SetTurnRightCount(int32 Count) { LyenlTurnRightCount = Count; }	
	void SetTurnLeftCount(int32 Count) { LyenlTurnLeftCount = Count; }
	int32 GetTurnRightCount() const { return LyenlTurnRightCount; }
	int32 GetTurnLeftCount() const { return LyenlTurnLeftCount; }	
	int32 GetFireAttackTimes() const { return FireAttackTimes; }
	void SetFireAttack(int32 InTimes) { FireAttackTimes = InTimes; }

protected:
	FVector TargetLocation = FVector::Zero();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdatePatrol(float DeltaTime) override;
	virtual void UpdateSuspicious(float DeltaTime) override;
	virtual void UpdateAlert(float DeltaTime) override;
	virtual void UpdateCombat(float DeltaTime) override;
	virtual void UpdateAimingBow(float DeltaTime) override;
	virtual void UpdateAimingBowUpper(float DeltaTime);
	virtual void UpdateExplosionAttack(float DeltaTime);
	virtual void UpdateDashAttack(float DeltaTime);
	virtual void UpdateFireAttack(float DeltaTime);
	virtual void UpdateHornAttack(float DeltaTime);
	virtual void UpdateRunningAttack(float DeltaTime);
	virtual void UpdateDead(float DeltaTime);
	virtual void UpdateRebound(float DeltaTime);
	virtual void UpdateRodeo(float DeltaTime);
	virtual void UpdateStun(float DeltaTime);
	virtual void UpdateReadyToAttack(float DeltaTime);
	virtual void UpdateTemp(float DeltaTime);
};
