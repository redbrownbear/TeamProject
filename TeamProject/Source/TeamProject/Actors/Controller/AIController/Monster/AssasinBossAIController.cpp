// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/AssasinBossAIController.h"
#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAssasinBossAIController::AAssasinBossAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UAssasinBossFSMComponent>(TEXT("AssasinLeaderFSMComponent"));
}

void AAssasinBossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssasinBossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AAssasinBossAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// 감지됨
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
		{
			MonsterFSMComponent->SetPlayer(Player);
			UE_LOG(LogTemp, Warning, TEXT("AAssasinBossAIController::OnPerceptionUpdated Player set Valid"));
			return;
		}
	}

}
