// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "ProjectileTableRow.generated.h"

USTRUCT()
struct TEAMPROJECT_API FProjectileTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

public:
	UPROPERTY(EditAnywhere, Category = "Target")
	TSubclassOf<AActor> TargetActorClass0 = nullptr;
	UPROPERTY(EditAnywhere, Category = "Target")
	TSubclassOf<AActor> TargetActorClass1 = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "Collision")
	TSubclassOf<UShapeComponent> CollisionClass/* = USphereComponent::StaticClass()*/;
	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionSphereRadius = 32.f;
	UPROPERTY(EditAnywhere, Category = "Collision")
	FVector CollisionBoxExtent = FVector(32.0, 32.0, 32.0);
	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float MaxSpeed = 0.f;
	UPROPERTY(EditAnywhere, Category = "Speed")
	float InitialSpeed = 0.f;

public:
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityScale = 0.f;

public:
	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 0.f;
};
