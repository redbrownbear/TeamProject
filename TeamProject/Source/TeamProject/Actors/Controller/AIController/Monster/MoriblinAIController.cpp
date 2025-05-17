// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/MoriblinAIController.h"
#include "Components/FSMComponent/MoriblinFSMComponent.h"

AMoriblinAIController::AMoriblinAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UMoriblinFSMComponent>(TEXT("BokoblinFSMComponent"));
}

void AMoriblinAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMoriblinAIController::BeginPlay()
{
	Super::BeginPlay();
}
