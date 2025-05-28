// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Temple/Ice/IcePillar.h"
#include "Components/BoxComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TimerManager.h"

// Sets default values
AIcePillar::AIcePillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	//StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PillarMesh(TEXT("/Game/Resources/Map/Dungeon/DgnObj_Ice.DgnObj_Ice"));
	StaticMeshComponent->SetStaticMesh(PillarMesh.Object);

	MaterialInterface = StaticMeshComponent->GetMaterial(0);
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInstance);
	DynamicMaterialInstance->SetScalarParameterValue("Alpha", 1.0f);

}

// Called when the game starts or when spawned
void AIcePillar::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	SetActorLocation(StartLocation - FVector(0, 0, MaxHeight));
	CurrentRise = 0.f;
	bIsRising = true;
}

// Called every frame
void AIcePillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsRising) return;

	const FVector CurrentLocation = GetActorLocation();

	// Stop

	const float fDistance = FVector::Dist(CurrentLocation, PivotLocation);

	if (fDistance < MaxHeight)
	{
		FVector NextLocation = CurrentLocation + RiseDirection * MaxSpeed * DeltaTime;
		SetActorLocation(NextLocation);
	}
	else
	{
		bIsRising = false;
	}
}

void AIcePillar::DestroyPillar()
{
	/*if (BreakEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), BreakEffect, GetActorLocation(), FRotator::ZeroRotator
		);
	}*/

	Destroy();
}

void AIcePillar::SetRiseDirection(FVector InDirection)
{
	RiseDirection = InDirection;
}

void AIcePillar::SetPivotLocation(FVector InPosition)
{
	PivotLocation = InPosition;
	const FVector ReverseDirection = RiseDirection * -1;
	PivotLocation += ReverseDirection * MaxHeight;
	SetActorLocation(PivotLocation);
}

