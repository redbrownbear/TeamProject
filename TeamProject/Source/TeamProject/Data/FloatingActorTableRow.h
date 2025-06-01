// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingActorTableRow.generated.h"

/**
 * 
 */
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
