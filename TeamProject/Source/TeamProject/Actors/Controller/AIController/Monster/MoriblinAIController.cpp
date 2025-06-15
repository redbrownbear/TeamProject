// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/MoriblinAIController.h"
#include "Components/FSMComponent/Monster/MoriblinFSMComponent.h"

#include "SubSystem/TimeManager.h"

AMoriblinAIController::AMoriblinAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UMoriblinFSMComponent>(TEXT("BokoblinFSMComponent"));
}

void AMoriblinAIController::Tick(float DeltaTime)
{
	static UTimeManagerSubsystem* TimeManager = GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>();
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::Tick(DeltaTime);
}

void AMoriblinAIController::BeginPlay()
{
	Super::BeginPlay();
}
