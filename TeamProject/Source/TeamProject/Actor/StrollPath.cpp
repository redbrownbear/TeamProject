// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/StrollPath.h"
#include "Components/SplineComponent.h"

// Sets default values
AStrollPath::AStrollPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

FVector AStrollPath::GetSplinePointLocation(int32 InIndex)
{
	if (InIndex < 0 || GetSplineMaxIndex() <= InIndex)
	{
		UE_LOG(LogTemp, Error, TEXT("APatrolPath::GetSplinePointVector // Index out of range"));
		check(false);
	}
	FVector Location = SplineComponent->GetLocationAtSplinePoint(InIndex, ESplineCoordinateSpace::World);

	return Location;
}

int32 AStrollPath::GetSplineMaxIndex() const
{
	if (!SplineComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AStrollPath::SplineComponent is nullptr!"));
		return 0;
	}

	return SplineComponent->GetNumberOfSplinePoints();
}

