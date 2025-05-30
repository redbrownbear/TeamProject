// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/AB_Stone.h"

// Sets default values
AAB_Stone::AAB_Stone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAB_Stone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAB_Stone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

