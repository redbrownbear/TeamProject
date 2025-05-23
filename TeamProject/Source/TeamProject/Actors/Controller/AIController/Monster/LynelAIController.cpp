// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/LynelAIController.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

ALynelAIController::ALynelAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<ULynelFSMComponent>(TEXT("LynelFSMComponent"));
}

void ALynelAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALynelAIController::BeginPlay()
{
	Super::BeginPlay();
}
