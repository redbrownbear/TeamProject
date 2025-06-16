// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"

#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Subsystem/TimeManager.h"

AMonsterAIController::AMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// 시야 설정
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = MONSTER_DEFAULT_SIGHT_RADIUS;
	SightConfig->LoseSightRadius = MONSTER_DEFAULT_LOSE_SIGHT_RADIUS;
	SightConfig->PeripheralVisionAngleDegrees = MONSTER_SIGHT_ANGLE; // Sight Degree
	SightConfig->SetMaxAge(1.0f); // Memory Time
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 감지 업데이트 이벤트 바인딩
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnPerceptionUpdated);
	//PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterFSMComponent)
	{
		APawn* TempPawn = GetPawn();
		if (APawnMonster* Monster = Cast<APawnMonster>(TempPawn))
		{
			MonsterFSMComponent->SetPawnMonster(Monster);
			MonsterFSMComponent->SetCampFire(Monster->GetCampFire());
			MonsterFSMComponent->SetPatrolPath(Monster->GetPatrolPath());
		}
		else if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(TempPawn))
		{
			MonsterFSMComponent->SetCharacterMonster(CharacterMonster);
			MonsterFSMComponent->SetPatrolPath(CharacterMonster->GetPatrolPath());
		}

		MonsterFSMComponent->SetPlayer(nullptr);
	}

	TimeManager = GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>();
}

void AMonsterAIController::Tick(float DeltaTime)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::Tick(CustumDeltaTime);
}

void AMonsterAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	APlayerCharacter* Player = nullptr;

	for (AActor* SeenActor : UpdatedActors)
	{
		if (SeenActor->IsA<APlayerCharacter>())
		{
			Player = Cast<APlayerCharacter>(SeenActor);
		}
		else if (SeenActor->IsA<AWorldWeapon>())
		{
			AWorldWeapon* WW = Cast<AWorldWeapon>(SeenActor);
			if (MonsterFSMComponent->GetPlayer())
			{
				if (!MonsterFSMComponent->IsToCatchWeapon())
				{
					if (MonsterFSMComponent->GetCurrentState() == EMonsterState::FindWeapon)
					{
						MonsterFSMComponent->SetToCatchWeapon(WW);
					}
					else if (MonsterFSMComponent->GetCurrentState() == EMonsterState::Combat)
					{
						MonsterFSMComponent->ChangeState(EMonsterState::FindWeapon);
						MonsterFSMComponent->SetToCatchWeapon(WW);
					}
				}
			}
		}
	}

	if (Player)
	{
		MonsterFSMComponent->SetPlayer(Player);
		UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Valid"));
	}
	//else
	//{
	//	MonsterFSMComponent->SetPlayer(nullptr);
	//	UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Null"));
	//}
}

//void AMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	if (Stimulus.WasSuccessfullySensed())
//	{
//		if (AWorldWeapon* WW = Cast<AWorldWeapon>(Actor))
//		{
//			if (MonsterFSMComponent->GetPlayer())
//			{
//				if (!MonsterFSMComponent->IsToCatchWeapon())
//				{
//					if (MonsterFSMComponent->GetCurrentState() == EMonsterState::FindWeapon)
//					{
//						MonsterFSMComponent->SetToCatchWeapon(WW);
//					}
//					else if (MonsterFSMComponent->GetCurrentState() == EMonsterState::Combat)
//					{
//						MonsterFSMComponent->ChangeState(EMonsterState::FindWeapon);
//						MonsterFSMComponent->SetToCatchWeapon(WW);
//					}
//				}
//			}
//		}
//
//
//		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
//		{
//			MonsterFSMComponent->SetPlayer(Player);
//			UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Valid"));
//			return;
//		}
//
//
//	}
//
//}
void AMonsterAIController::SetAIEnabled(bool bEnabled)
{
}

