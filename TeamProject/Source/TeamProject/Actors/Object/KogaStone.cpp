// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/KogaStone.h"

// Sets default values
AKogaStone::AKogaStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKogaStone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKogaStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

