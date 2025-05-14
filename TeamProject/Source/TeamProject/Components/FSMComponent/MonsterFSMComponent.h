// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Utils.h"
#include "MonsterFSMComponent.generated.h"

class AMonster;

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
protected:
	UPROPERTY()
	TObjectPtr<AMonster> Owner = nullptr;

protected:
	EMonsterState			eCurrentState;
	EMonsterGroupType		eGroupType;
	EMonsterMovementState	eMovementState;
public:
	EMonsterState GetMonsterState() const { return eCurrentState; }
	EMonsterGroupType GetMonsterGroupType() const { return eGroupType; }
	EMonsterMovementState GetMonsterMovementState() const { return eMovementState; }
	void SetMonsterState(EMonsterState NewState) { eCurrentState = NewState; }	
	void SetMonsterGroupType(EMonsterGroupType NewGroupType) { eGroupType = NewGroupType; }	
	void SetMonsterMovementState(EMonsterMovementState NewMovementState) { eMovementState = NewMovementState; }

protected:
	float SuspicionGauge = 0.0f;
	float MaxSuspicionGauge = 100.0f;

protected:
	int32 CurrentPatrolIndex = 0;

private:
	void HandleState(float DeltaTime);
protected:
	void ChangeState(EMonsterState NewState);

	virtual void UpdateIdle(float DeltaTime);
	virtual void UpdatePatrol(float DeltaTime);
	virtual void UpdateSuspicious(float DeltaTime);
	virtual void UpdateAlert(float DeltaTime);
	virtual void UpdateCombat(float DeltaTime);

protected:
	void MoveToLocation(const FVector& InLocation);
};
