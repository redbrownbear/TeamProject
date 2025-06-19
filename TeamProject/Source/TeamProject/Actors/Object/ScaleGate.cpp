// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/ScaleGate.h"
#include "SubSystem/Puzzle/EventManager.h"

// Sets default values
AScaleGate::AScaleGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetRelativeScale3D(FVector(90.f, 90.f, 90.f));
}

// Called when the game starts or when spawned
void AScaleGate::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}
}

void AScaleGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpenGate)
	{
		MoveGateTick(DeltaTime);
	}
}

void AScaleGate::OpenScaleGate()
{
	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + FVector::UpVector * GetComponentsBoundingBox().GetSize().Z;
	bOpenGate = true;
}

void AScaleGate::MoveGateTick(float DeltaTime)
{
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, DeltaTime);
	StaticMeshComponent->SetWorldLocation(NewLocation);
	StartLocation = NewLocation;
}
