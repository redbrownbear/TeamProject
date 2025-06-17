// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/Npc/Musician/MusicianController.h"
#include "Components/FSMComponent/Npc/Musician/MusicianFSMComponent.h"

AMusicianController::AMusicianController()
{
	NpcFSMComponent = CreateDefaultSubobject<UMusicianFSMComponent>(TEXT("MusicianFSMComponent"));
}

void AMusicianController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMusicianController::BeginPlay()
{
	Super::BeginPlay();
}
