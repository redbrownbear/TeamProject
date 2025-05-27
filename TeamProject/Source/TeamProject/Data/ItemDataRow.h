// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "ItemDataRow.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EItemCategoryType ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftObjectPtr<UTexture2D> CategoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Description;


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMesh* StaticMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    FTransform Transform = FTransform::Identity;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TSubclassOf<UShapeComponent> CollisionClass/* = USphereComponent::StaticClass()*/;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionSphereRadius = 32.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector CollisionBoxExtent = FVector(32.0, 32.0, 32.0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
	float Damage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Kind")
	EWeaponKind eWeaponKind;
};