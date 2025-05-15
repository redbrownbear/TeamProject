// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/CampFire.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Misc/Utils.h"

// Sets default values
ACampFire::ACampFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset
    { TEXT("/Script/Engine.StaticMesh'/Game/Assets/Object/FireWood/FireWood.FireWood'") };
    if (Asset.Object)
    {
        StaticMeshComponent->SetStaticMesh(Asset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACampFire::ACampFire // No FireWood StaticMeshAsset"));
    }

    StaticMeshComponent->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE, DEFAULT_MESH_SCALE, DEFAULT_MESH_SCALE));

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireAsset
    { TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Free_Fire/Shared/Particles/NS_Fire_Floor_01_Simple.NS_Fire_Floor_01_Simple'") };

    if (FireAsset.Object)
    {
        NiagaraComponent->SetAsset(FireAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACampFire::ACampFire // No Fire NiagaraAsset"));
    }
}

// Called when the game starts or when spawned
void ACampFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACampFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

