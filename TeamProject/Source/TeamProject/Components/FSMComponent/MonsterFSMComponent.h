// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Utils.h"
#include "MonsterFSMComponent.generated.h"

class AMonster;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UMonsterFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterFSMComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetOwner(AMonster* InOwner) { Owner = InOwner; }
	void SetPlayer(APlayerCharacter* InPlayer) { Player = InPlayer; }
protected:
	UPROPERTY()
	TObjectPtr<AMonster> Owner = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player = nullptr;

protected:
	EMonsterState			eCurrentState;
	EMonsterGroupType		eGroupType;

public:
	EMonsterState GetMonsterState() const { return eCurrentState; }
	EMonsterGroupType GetMonsterGroupType() const { return eGroupType; }

	void SetMonsterState(EMonsterState NewState) { eCurrentState = NewState; }	
	void SetMonsterGroupType(EMonsterGroupType NewGroupType);

protected:
	float SuspicionGauge = 0.0f;
	float MaxSuspicionGauge = MONSTER_MAX_SUSPICIOUS_GAUGE;

	float SignalElapsedTime = 0.f;

protected:
	int32 CurrentPatrolIndex = 0;

private:
	void HandleState(float DeltaTime);
public:
	void ChangeState(EMonsterState NewState);

protected:
	virtual void UpdateIdle(float DeltaTime);
	virtual void UpdateDance(float DeltaTime);
	virtual void UpdateToDance(float DeltaTime);
	virtual void UpdatePatrol(float DeltaTime);
	virtual void UpdateSuspicious(float DeltaTime);
	virtual void UpdateAlert(float DeltaTime);
	virtual void UpdateCombat(float DeltaTime);
	virtual void UpdateSignal(float DeltaTime);

protected:
	void MoveToLocation(const FVector& InLocation);
	void StopMove();
};
