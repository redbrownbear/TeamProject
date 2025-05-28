// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/HinoxAIController.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"

AHinoxAIController::AHinoxAIController()
{
	MonsterFSMComponent = CreateDefaultSubobject<UHinoxFSMComponent>(TEXT("HinoxFSMComponent"));
}

void AHinoxAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHinoxAIController::BeginPlay()
{
	Super::BeginPlay();	
}
