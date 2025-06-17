// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/Npc/Scientist/ScientistController.h"
#include "Components/FSMComponent/Npc/Scientist/ScientistFSMComponent.h"

AScientistController::AScientistController()
{
	NpcFSMComponent = CreateDefaultSubobject<UScientistFSMComponent>(TEXT("ScientistFSMComponent"));
}

void AScientistController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScientistController::BeginPlay()
{
	Super::BeginPlay();
}
