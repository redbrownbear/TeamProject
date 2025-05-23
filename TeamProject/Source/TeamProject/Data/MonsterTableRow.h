// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Utils.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
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
	UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* APPEAR = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* BOW_UPPER_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* BOW_UPPER_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_DASH_LSWORD_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_DASH_LSWORD_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_DASH_SWORD_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_DASH_SWORD_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_EXPLOSION_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_EXPLOSION_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_FIRE_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_FIRE = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_FIRE_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_HORN_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_HORN_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_RUNNING_LSWORD_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_RUNNING_LSWORD_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_RUNNING_SWORD_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* ATTACK_RUNNING_SWORD_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* REBOUND = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* RODEO_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* RODEO_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* STUN_START = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* STUN_END = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* TURN_180_L = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* TURN_180_R = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* DRAW_BOW = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* SHEATH_BOW = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* BOW_TO_SWORD = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* SWORD_TO_BOW = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* DRAW_LSWORD = nullptr;
    UPROPERTY(EditAnywhere, Category = "Monster|Animation")
	UAnimMontage* SHEATH_LSWORD = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "Monster|AI")
	TSubclassOf<AMonsterAIController> AIControllerClass;

public:
	UPROPERTY(EditAnywhere, Category = "NiagaraEffect", meta = (RowType = "NiagaraEffectTableRow"))
	FDataTableRowHandle NiagaraEffectTableRowHandle;

public:
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (RowType = "ItemData"))
	FDataTableRowHandle MeleeWeaponTableRowHandle;

public:
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (RowType = "ItemData"))
	FDataTableRowHandle BowWeaponTableRowHandle;

};
