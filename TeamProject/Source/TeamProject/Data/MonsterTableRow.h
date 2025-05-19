// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Utils.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Components/FSMComponent/MonsterFSMComponent.h"
#include "MonsterTableRow.generated.h"

USTRUCT()
struct TEAMPROJECT_API FMonsterTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Monster")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Monster")
	FTransform MeshTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	TSubclassOf<UAnimInstance> AnimClass;

public:
	UPROPERTY(EditAnywhere, Category = "Monster|Collision")
	float CollisionSphereRadius = 32.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Monster|Movement")
	float WalkMovementMaxSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Monster|Movement")
	float RunMovementMaxSpeed = 300.f;

public: // FSM
	UPROPERTY(EditAnywhere, Category = "Monster|FSM")
	EMonsterGroupType eMonsterGroupType = EMonsterGroupType::TreasureBox;


public: // Animation
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AttackMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AttackBigMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AttackSpearMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AttackSwordMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AttackLSwordMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AttackThrowMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* JumpStartMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* JumpEndMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* DamageMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* AngryMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* BowStartMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* BowEndMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ThrowMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* DanceStartMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* DanceEndMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* WeaponCatchMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* FindMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* SignalStartMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* SignalEndMontage = nullptr;


public:
	UPROPERTY(EditAnywhere, Category = "Monster|AI")
	TSubclassOf<AMonsterAIController> AIControllerClass;

public:
	UPROPERTY(EditAnywhere, Category = "NiagaraEffect", meta = (RowType = "NiagaraEffectTableRow"))
	FDataTableRowHandle NiagaraEffectTableRowHandle;

};
