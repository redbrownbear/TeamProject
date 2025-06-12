// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/Scale.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScale::AScale()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    SeesawMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SeesawMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/MetalActors/PM_MetalActor.PM_MetalActor"));
    PhysicalMaterial = PhysMaterial.Object;

    if (PhysMaterial.Succeeded())
    {
        PhysicalMaterial = PhysMaterial.Object;
        SeesawMesh->SetPhysMaterialOverride(PhysicalMaterial);
    }

    SeesawMesh->SetSimulatePhysics(true);
    SeesawMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SeesawMesh->SetCollisionObjectType(ECC_PhysicsBody);
    SeesawMesh->SetNotifyRigidBodyCollision(true);
}
