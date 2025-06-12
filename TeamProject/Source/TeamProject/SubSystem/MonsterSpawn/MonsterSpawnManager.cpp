// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/MonsterSpawn/MonsterSpawnManager.h"
#include "Actors/Monster/MonsterSpawnPoint.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Monster/CharacterMonster.h"

#include "Kismet/GameplayStatics.h"

#include "Misc/Utils.h"

TArray<FMonsterInfo> UMonsterSpawnManager::GetSpawnInfoArray(FName LevelName)
{
    bool* bInited = LevelInit.Find(LevelName);


    if (nullptr == bInited)
    {
        UE_LOG(LogTemp, Error, TEXT("UMonsterSpawnManager::GetSpawnInfoArray // LevelInit.Find failed, Invalid LevelName"));
        check(false);
        return TArray<FMonsterInfo>();
    }

    // Didn't inited yet
    if (!(*bInited))
    {
        LevelInit[LevelName] = true;

        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), OutActors);

        for (AActor* IterActor : OutActors)
        {
            if (AMonsterSpawnPoint* MonsterSpawnPoint = Cast<AMonsterSpawnPoint>(IterActor))
            {
                FMonsterInfo Info;
                Info.Transform = IterActor->GetActorTransform();
                Info.MonsterDataRow = MonsterSpawnPoint->DataTableRowHandle;
                Info.SpawnPointGuid = MonsterSpawnPoint->SpawnPointGuid;
                Info.bIsAlive = MonsterSpawnPoint->bIsAlive;

                SpawnInfos[LevelName].Add(Info);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("UMonsterSpawnManager::GetSpawnInfoArray // Cast<AMonsterSpawnPoint>() Failed"));
                check(false);
            }
        }
    }


    return SpawnInfos[LevelName];
}
