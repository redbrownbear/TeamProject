// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/HinoxAIController.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"

#include "SubSystem/TimeManager.h"

AHinoxAIController::AHinoxAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UHinoxFSMComponent>(TEXT("HinoxFSMComponent"));
}

void AHinoxAIController::Tick(float DeltaTime)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::Tick(CustumDeltaTime);
}

void AHinoxAIController::BeginPlay()
{
	Super::BeginPlay();	
}
