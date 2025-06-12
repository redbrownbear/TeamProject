// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor_GameMap.h"

#include "Subsystem/MonsterSpawn/MonsterSpawnManager.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"

#include "Misc/Utils.h"

void ALevelScriptActor_GameMap::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UMonsterSpawnManager* MonsterSpawnManager = GameInstance->GetSubsystem<UMonsterSpawnManager>())
			{
				const FString LevelName = World->GetName();
				TArray<FMonsterInfo> MonsterInfoArray = MonsterSpawnManager->GetSpawnInfoArray(FName(*LevelName));

				for (const FMonsterInfo& Info : MonsterInfoArray)
				{
					if (Info.bIsAlive)
					{
						const FName MonsterName = Info.MonsterDataRow.RowName;
						if (MonsterName == MonsterName::AssasinBoss
							|| MonsterName == MonsterName::AssasinLeader
							|| MonsterName == MonsterName::Hinox
							|| MonsterName == MonsterName::Lynel
							)
						{
							ACharacterMonster* CharacterMonster = World->SpawnActorDeferred<ACharacterMonster>(ACharacterMonster::StaticClass(),
								FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
							CharacterMonster->SetData(Info.MonsterDataRow);
							CharacterMonster->FinishSpawning(Info.Transform);
						}
						else
						{
							APawnMonster* PawnMonster = World->SpawnActorDeferred<APawnMonster>(APawnMonster::StaticClass(),
								FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
							PawnMonster->SetData(Info.MonsterDataRow);
							PawnMonster->FinishSpawning(Info.Transform);
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ALevelScriptActor_GameMap::BeginPlay // No MonsterSpawnManager"));
				check(false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ALevelScriptActor_GameMap::BeginPlay // No GameInstance"));
			check(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ALevelScriptActor_GameMap::BeginPlay // No World"));
		check(false);
	}
}
