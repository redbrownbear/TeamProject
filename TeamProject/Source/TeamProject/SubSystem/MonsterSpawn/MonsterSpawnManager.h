// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MonsterSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct TEAMPROJECT_API FMonsterInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid SpawnPointGuid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid PatrolPathPointGuid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid CampFirePointGuid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDataTableRowHandle MonsterDataRow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Transform;




    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAlive = true;
};

UCLASS()
class TEAMPROJECT_API UMonsterSpawnManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	// <Level, <MonsterInfo, bAlive>> 
	TMap<FName, TArray<FMonsterInfo>> SpawnInfos;
    TMap<FName, bool> LevelInit;

public:
    TArray<FMonsterInfo> GetSpawnInfoArray(FName LevelName);
};
