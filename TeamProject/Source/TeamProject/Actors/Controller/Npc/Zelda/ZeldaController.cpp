// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/Npc/Zelda/ZeldaController.h"
#include "Components/FSMComponent/Npc/Zelda/ZeldaFSMComponent.h"

AZeldaController::AZeldaController()
{
	NpcFSMComponent = CreateDefaultSubobject<UZeldaFSMComponent>(TEXT("ZeldaFSMComponent"));
}

void AZeldaController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZeldaController::BeginPlay()
{
	Super::BeginPlay();
}
