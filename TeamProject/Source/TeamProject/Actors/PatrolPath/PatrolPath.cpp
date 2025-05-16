// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PatrolPath/PatrolPath.h"
#include "Components/SplineComponent.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

FVector APatrolPath::GetSplinePointLocation(int32 InIndex)
{
	if (InIndex < 0 || GetSplineMaxIndex() <= InIndex)
	{
		UE_LOG(LogTemp, Error, TEXT("APatrolPath::GetSplinePointVector // Index out of range"));
		check(false);
	}
	FVector Location = SplineComponent->GetLocationAtSplinePoint(InIndex, ESplineCoordinateSpace::World);

	return Location;
}

int32 APatrolPath::GetSplineMaxIndex() const
{
	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	return NumPoints;
}

