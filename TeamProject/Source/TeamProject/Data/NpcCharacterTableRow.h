// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "NpcCharacterTableRow.generated.h"

USTRUCT()
struct TEAMPROJECT_API FNpcCharacterTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Npc")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Npc")
	FTransform MeshTransform = FTransform::Identity;
	
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	TSubclassOf<UAnimInstance> AnimClass;

public:
	UPROPERTY(EditAnywhere, Category = "Npc|Collision")
	float CollisionSphereRadius = 32.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Npc|Movement")
	float WalkMovementMaxSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Npc|Movement")
	float RunMovementMaxSpeed = 400.f;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* IdleMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* SitMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* StandMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* WalkMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* RunMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* TalkMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* HideMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Npc|Animation")
	UAnimMontage* SellMontage = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "Npc|AI")
	TSubclassOf<ANpcController> NpcControllerClass;

};
