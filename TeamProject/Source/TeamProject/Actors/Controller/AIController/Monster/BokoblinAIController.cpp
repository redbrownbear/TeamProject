// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/BokoblinAIController.h"
#include "Components/FSMComponent/Monster/BokoblinFSMComponent.h"

ABokoblinAIController::ABokoblinAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UBokoblinFSMComponent>(TEXT("BokoblinFSMComponent"));
}

void ABokoblinAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABokoblinAIController::BeginPlay()
{
	Super::BeginPlay();
}
