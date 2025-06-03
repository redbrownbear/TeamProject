// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Misc/Utils.h"
#include "MonsterInterface.generated.h"

class UMonsterStatusComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAdvancedFloatingPawnMovement;
class UMonsterFSMComponent;
class USphereComponent;
class APatrolPath;
class ACampFire;
class UAnimInstance;
class USkeletalMeshComponent;

struct FMonsterTableRow;

UINTERFACE()
class UMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAMPROJECT_API IMonsterInterface
{
	GENERATED_BODY()
	
public:
	virtual UMonsterStatusComponent* GetStatusComponent() const = 0;

public:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) = 0;
protected:
	virtual FMonsterTableRow* GetMonsterData() const = 0;

public:
	void PlayMontage(EMonsterMontage _InEnum, bool bIsLoop = false);
	bool IsMontage(EMonsterMontage _InEnum);
	bool IsPlayingMontage(EMonsterMontage _InEnum = EMonsterMontage::END);


public:
	virtual APatrolPath* GetPatrolPath() const = 0;
	virtual ACampFire* GetCampFire() const = 0;
	virtual UAnimInstance* GetAnimInstance() const = 0;
	virtual USkeletalMeshComponent* GetMonsterMesh() const = 0;
	virtual UMonsterFSMComponent* GetFSMComponent() const = 0;
	virtual void SetSpeedWalk() = 0;
	virtual void SetSpeedRun() = 0;

public:
	virtual void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, int32 iOption = 0);

protected:
	virtual void OnDie();
	virtual void OnDeadEnd();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) = 0;

public:
	FName GetName() const;
	float GetDamageFromWeapon();
};
