// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/Npc/Heir/HeirController.h"
#include "Components/FSMComponent/Npc/Heir/HeirFSMComponent.h"

AHeirController::AHeirController()
{
	NpcFSMComponent = CreateDefaultSubobject<UHeirFSMComponent>(TEXT("HeirFSMComponent"));

}

void AHeirController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeirController::BeginPlay()
{
	Super::BeginPlay();

}