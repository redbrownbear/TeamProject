// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/MonsterSpawn/MonsterSpawnManager.h"
#include "Actors/Monster/MonsterSpawnPoint.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Monster/CharacterMonster.h"

#include "Kismet/GameplayStatics.h"

#include "Misc/Utils.h"

TArray<FMonsterInfo> UMonsterSpawnManager::GetSpawnInfoArray(FName LevelName)
{
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), OutActors);

    SpawnInfos.Empty();

    for (AActor* IterActor : OutActors)
    {
        if (AMonsterSpawnPoint* MonsterSpawnPoint = Cast<AMonsterSpawnPoint>(IterActor))
        {
            FMonsterInfo Info;
            Info.Transform = IterActor->GetActorTransform();
            Info.MonsterDataRow = MonsterSpawnPoint->DataTableRowHandle;
            Info.SpawnPointGuid = MonsterSpawnPoint->SpawnPointGuid;
            Info.PatrolPathPointGuid = MonsterSpawnPoint->PatrolPathPointGuid;
            Info.CampFirePointGuid = MonsterSpawnPoint->CampFirePointGuid;
            Info.bIsAlive = MonsterSpawnPoint->bIsAlive;

            SpawnInfos.Add(Info);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UMonsterSpawnManager::GetSpawnInfoArray // Cast<AMonsterSpawnPoint>() Failed"));
            check(false);
        }
    }


    return SpawnInfos;
}
