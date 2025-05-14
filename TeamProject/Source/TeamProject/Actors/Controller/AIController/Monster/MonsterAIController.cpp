// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Monster/Monster.h"
#include "Components/FSMComponent/MonsterFSMComponent.h"

AMonsterAIController::AMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	//MonsterFSMComponent = CreateDefaultSubobject<UMonsterFSMComponent>(TEXT("MonsterFSMComponent"));
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterFSMComponent)
	{
		MonsterFSMComponent->SetOwner(Cast<AMonster>(GetPawn()));
	}
}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


