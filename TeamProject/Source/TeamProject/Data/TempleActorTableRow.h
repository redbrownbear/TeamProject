// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TempleActorTableRow.generated.h"

/**
 * 
 */
USTRUCT()
struct TEAMPROJECT_API FTempleActorTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Actor")
	FString ActorName;

	UPROPERTY(EditAnywhere, Category = "Actor")
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform MeshTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform CollisionTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollisionProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MassInKg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LinearDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Friction;
};

USTRUCT()
struct TEAMPROJECT_API FFloatingActorTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Actor")
	FString ActorName;

	UPROPERTY(EditAnywhere, Category = "Actor")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform MeshTransform;
};
