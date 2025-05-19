// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Monster/Monster.h"
#include "Actor/Character/PlayerCharacter.h"


#include "Components/FSMComponent/MonsterFSMComponent.h"

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
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnPerceptionUpdated);

}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterFSMComponent)
	{
		MonsterFSMComponent->SetOwner(Cast<AMonster>(GetPawn()));
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
	}
	else
	{
		MonsterFSMComponent->SetPlayer(nullptr);
	}
}

void AMonsterAIController::SetAIEnabled(bool bEnabled)
{
}

