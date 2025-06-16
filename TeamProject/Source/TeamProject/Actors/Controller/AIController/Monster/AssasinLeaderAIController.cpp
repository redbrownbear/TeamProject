// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/AssasinLeaderAIController.h"
#include "Components/FSMComponent/Monster/AssasinLeaderFSMComponent.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "SubSystem/TimeManager.h"

AAssasinLeaderAIController::AAssasinLeaderAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UAssasinLeaderFSMComponent>(TEXT("AssasinLeaderFSMComponent"));
}

void AAssasinLeaderAIController::Tick(float DeltaTime)
{
	static UTimeManagerSubsystem* TimeManager = GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>();
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();


	Super::Tick(DeltaTime);
}

void AAssasinLeaderAIController::BeginPlay()
{
	Super::BeginPlay();
}

//void AAssasinLeaderAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	if (Stimulus.WasSuccessfullySensed())
//	{
//		// 감지됨
//		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
//		{
//			MonsterFSMComponent->SetPlayer(Player);
//			UE_LOG(LogTemp, Warning, TEXT("AAssasinLeaderAIController::OnPerceptionUpdated Player set Valid"));
//			return;
//		}
//	}
//}
