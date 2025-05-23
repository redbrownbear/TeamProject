// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Monster/Monster.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"


#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


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
	//PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterFSMComponent)
	{
		MonsterFSMComponent->SetOwner(Cast<AMonster>(GetPawn()));
		MonsterFSMComponent->SetPlayer(nullptr);
	}
}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	APlayerCharacter* Player = nullptr;

	for (AActor* SeenActor : UpdatedActors)
	{
		if (APlayerCharacter* DetectedPlayer = Cast<APlayerCharacter>(SeenActor))
		{
			Player = DetectedPlayer;
			break; // 플레이어 감지
		}
	}

	if (Player)
	{
		MonsterFSMComponent->SetPlayer(Player);
		UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Valid"));
	}
	else
	{
		MonsterFSMComponent->SetPlayer(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Null"));
	}
}

void AMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// 감지됨
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
		{
			MonsterFSMComponent->SetPlayer(Player);
			UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Valid"));
			return;
		}
		else if (AWorldWeapon* WW = Cast<AWorldWeapon>(Actor))
		{
			if (!MonsterFSMComponent->IsToCatchWeapon())
			{
				if (MonsterFSMComponent->GetMonsterState() == EMonsterState::FindWeapon)
				{
					MonsterFSMComponent->SetToCatchWeapon(WW);
				}
				else if (MonsterFSMComponent->GetMonsterState() == EMonsterState::Combat)
				{
					MonsterFSMComponent->ChangeState(EMonsterState::FindWeapon);
					MonsterFSMComponent->SetToCatchWeapon(WW);
				}
			}
		}

	}
	//else
	//{
	//	// 감지 해제됨
	//	MonsterFSMComponent->SetPlayer(nullptr);
	//	UE_LOG(LogTemp, Warning, TEXT("AMonsterAIController::OnPerceptionUpdated Player set Null"));
	//}
}
void AMonsterAIController::SetAIEnabled(bool bEnabled)
{
}

