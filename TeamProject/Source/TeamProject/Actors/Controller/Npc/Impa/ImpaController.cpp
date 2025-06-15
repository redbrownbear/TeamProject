// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/Npc/Impa/ImpaController.h"
#include "Components/FSMComponent/Npc/Impa/ImpaFSMComponent.h"

AImpaController::AImpaController()
{
	NpcFSMComponent = CreateDefaultSubobject<UImpaFSMComponent>(TEXT("ImpaFSMComponent"));

}

void AImpaController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AImpaController::BeginPlay()
{
	Super::BeginPlay();
	
}
