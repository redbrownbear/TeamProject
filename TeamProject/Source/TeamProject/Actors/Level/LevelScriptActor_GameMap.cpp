// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor_GameMap.h"

#include "Subsystem/MonsterSpawn/MonsterSpawnManager.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Object/CampFire.h"
#include "Actors/Object/PatrolPath.h"

#include "Kismet/GameplayStatics.h"

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

				TArray<AActor*> PatrolPathActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), PatrolPathActors);

				TArray<AActor*> CampFireActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACampFire::StaticClass(), CampFireActors);

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

							for (AActor* PatrolPathIterActor : PatrolPathActors)
							{
								if (APatrolPath* PatrolPath = Cast<APatrolPath>(PatrolPathIterActor))
								{
									const FGuid PatrolPathGuid = PatrolPath->GetPatrolPathGuid();
									if (PatrolPathGuid == Info.PatrolPathPointGuid)
									{
										CharacterMonster->SetPatrolPath(PatrolPath);
									}
								}
							}

							CharacterMonster->SetData(Info.MonsterDataRow);
							CharacterMonster->FinishSpawning(Info.Transform);
						}
						else
						{
							APawnMonster* PawnMonster = World->SpawnActorDeferred<APawnMonster>(APawnMonster::StaticClass(),
								FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


							for (AActor* CampFireIterActor : CampFireActors)
							{
								if (ACampFire* CampFire = Cast<ACampFire>(CampFireIterActor))
								{
									const FGuid CampFireGuid = CampFire->GetCampFireGuid();
									if (CampFireGuid == Info.CampFirePointGuid)
									{
										PawnMonster->SetCampFire(CampFire);
									}
								}
							}



							for (AActor* PatrolPathIterActor : PatrolPathActors)
							{
								if (APatrolPath* PatrolPath = Cast<APatrolPath>(PatrolPathIterActor))
								{
									const FGuid PatrolPathGuid = PatrolPath->GetPatrolPathGuid();
									if (PatrolPathGuid == Info.PatrolPathPointGuid)
									{
										PawnMonster->SetPatrolPath(PatrolPath);
									}
								}
							}

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
