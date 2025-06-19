// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Temple/Surface/WaterSurface.h"
#include "Components/BoxComponent.h"

// Sets default values
AWaterSurface::AWaterSurface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset
    (TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    if (Asset.Object)
    {
        StaticMeshComponent->SetStaticMesh(Asset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AWaterSurface::AWaterSurface // No WaterSurface StaticMeshAsset"));
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material
    (TEXT("/DatasmithContent/Materials/Water/M_Water.M_Water"));
    if (Material.Succeeded())
    {
        StaticMeshComponent->SetMaterial(0, Material.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AWaterSurface::AWaterSurface // No WaterSurface Material"));
    }
}

// Called when the game starts or when spawned
void AWaterSurface::BeginPlay()
{
	Super::BeginPlay();
	
}

