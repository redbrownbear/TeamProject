// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetalActorTableRow.generated.h"

USTRUCT()
struct TEAMPROJECT_API FMetalActorTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	FString ActorName;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FTransform MeshTransform;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TSubclassOf<UShapeComponent> CollisionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionSphereRadius = 32.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector CollisionBoxExtent = FVector(32.0, 32.0, 32.0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionCapsuleRadius = 22.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionCapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FName CollisionProfileName;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MassInKg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float LinearDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float Friction;
};