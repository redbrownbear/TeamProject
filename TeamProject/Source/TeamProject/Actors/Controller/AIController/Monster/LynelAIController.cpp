// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/LynelAIController.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

#include "SubSystem/TimeManager.h"

ALynelAIController::ALynelAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<ULynelFSMComponent>(TEXT("LynelFSMComponent"));
}

void ALynelAIController::Tick(float DeltaTime)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::Tick(CustumDeltaTime);
}

void ALynelAIController::BeginPlay()
{
	Super::BeginPlay();
}
