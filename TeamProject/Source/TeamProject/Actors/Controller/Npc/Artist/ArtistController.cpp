// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/Npc/Artist/ArtistController.h"
#include "Components/FSMComponent/Npc/Artist/ArtistFSMComponent.h"

AArtistController::AArtistController()
{
	NpcFSMComponent = CreateDefaultSubobject<UArtistFSMComponent>(TEXT("ArtistFSMComponent"));
}

void AArtistController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArtistController::BeginPlay()
{
	Super::BeginPlay();
}
