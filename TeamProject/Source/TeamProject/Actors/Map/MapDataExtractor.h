// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapDataExtractor.generated.h"

USTRUCT(BlueprintType)
struct FMapTileInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector WorldLocation;

	UPROPERTY(BlueprintReadOnly)
	float Height;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWalkable; // 판단 기준은 원하는 로직 삽입
};

UCLASS()
class TEAMPROJECT_API AMapDataExtractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapDataExtractor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, Category = "MiniMap")
	void ExtractLandscapeData();

private:
	float GetHeightAtLocation(const FVector2D& XY);


public:
	const TArray<FMapTileInfo>& GetMapTiles() { return MapTiles; }

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FMapTileInfo> MapTiles;
};
