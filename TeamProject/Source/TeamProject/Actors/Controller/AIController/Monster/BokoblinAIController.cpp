// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/BokoblinAIController.h"
#include "Components/FSMComponent/Monster/BokoblinFSMComponent.h"

#include "SubSystem/TimeManager.h"

ABokoblinAIController::ABokoblinAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UBokoblinFSMComponent>(TEXT("BokoblinFSMComponent"));
}

void ABokoblinAIController::Tick(float DeltaTime)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::Tick(CustumDeltaTime);
}

void ABokoblinAIController::BeginPlay()
{
	Super::BeginPlay();
}
