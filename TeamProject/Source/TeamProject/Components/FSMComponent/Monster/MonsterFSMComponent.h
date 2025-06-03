// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Utils.h"
#include "MonsterFSMComponent.generated.h"

class APawnMonster;
class ACharacterMonster;
class APlayerCharacter;
class AWorldWeapon;
class IMonsterInterface;
class ACampFire;
class APatrolPath;

UCLASS()
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
	void SetPawnMonster(APawnMonster* InOwner) { PawnMonster = InOwner; }
	void SetCharacterMonster(ACharacterMonster* InOwner) { CharacterMonster = InOwner; }
	void SetPlayer(APlayerCharacter* InPlayer) { Player = InPlayer; }
	void OnHitReceived(bool bIsDead = false);
	UFUNCTION()
	void BindHitEvent();


public:
	void SetToCatchWeapon(AWorldWeapon* InWW) { ToCatchWeapon = InWW; }
	void SetCatchedWeapon(AWorldWeapon* InWW) { CurrentWeapon = InWW; }
	bool IsToCatchWeapon() { return ToCatchWeapon ? true : false; }
	void SetMeleeWeapon(AWorldWeapon* InWW) { MeleeWeapon = InWW; }
	void SetBowWeapon(AWorldWeapon* InWW) { BowWeapon = InWW; }
	void SetPatrolPath(APatrolPath* InPatrolPath) { PatrolPath = InPatrolPath; }
	void SetCampFire(ACampFire* InCampFire) { CampFire = InCampFire; }

public:
	void SheathMeleeWeapon();
	void SheathBowWeapon();
	void DrawMeleeWeapon();
	void DrawBowWeapon();
	const AWorldWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	const AWorldWeapon* GetMeleeWeapon() { return MeleeWeapon; }
	const AWorldWeapon* GetBowWeapon() { return BowWeapon; }

protected:
	void DropWeapons();

protected:
	UPROPERTY()
	TObjectPtr<ACharacterMonster> CharacterMonster = nullptr;
	UPROPERTY()
	TObjectPtr<APawnMonster> PawnMonster = nullptr;
	UPROPERTY()
	TObjectPtr<ACampFire> CampFire = nullptr;
	UPROPERTY()
	TObjectPtr<APatrolPath> PatrolPath = nullptr;




	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player = nullptr;
	UPROPERTY()
	TObjectPtr<AWorldWeapon> ToCatchWeapon = nullptr;
	UPROPERTY()
	TObjectPtr<AWorldWeapon> CurrentWeapon = nullptr;
	UPROPERTY()
	TObjectPtr<AWorldWeapon> MeleeWeapon = nullptr;
	UPROPERTY()
	TObjectPtr<AWorldWeapon> BowWeapon = nullptr;

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
	float SignalElapsedTime = 0.f;
	float AimingBowElapsedTime = 0.f;


protected:
	int32 CurrentPatrolIndex = 0;

protected:
	float CurrentAttackCoolTime = 0.f;

protected:
	virtual void HandleState(float DeltaTime);
public:
	virtual void ChangeState(EMonsterState NewState);

protected:
	virtual void UpdateIdle(float DeltaTime);
	virtual void UpdateDance(float DeltaTime);
	virtual void UpdateToDance(float DeltaTime);
	virtual void UpdatePatrol(float DeltaTime);
	virtual void UpdateSuspicious(float DeltaTime);
	virtual void UpdateAlert(float DeltaTime);
	virtual void UpdateFindWeapon(float DeltaTime);
	virtual void UpdateCombat(float DeltaTime);
	virtual void UpdateSignal(float DeltaTime);
	virtual void UpdateAimingBow(float DeltaTime);
	virtual void UpdateDying(float DeltaTime);
	virtual void UpdateDamage(float DeltaTime);

protected:
	void MoveToLocation(const FVector& InLocation);
	void StopMove();
	void SpawnProjectile(FName ProjectileName, FName CollisionProfileName);
	UFUNCTION()
	void UpdateUIHPBar(float CurrentHP, float MaxHP);


};
