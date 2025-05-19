// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/MapMeshWithMutlipleStaticMesh.h"
#include "Misc/Utils.h"

AMapMeshWithMutlipleStaticMesh::AMapMeshWithMutlipleStaticMesh()
{
	PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AMapMeshWithMutlipleStaticMesh::BeginPlay()
{
	Super::BeginPlay();
}

void AMapMeshWithMutlipleStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapMeshWithMutlipleStaticMesh::AddDecorationMesh(UStaticMesh* Mesh)
{
    FString Name = FString::Printf(TEXT("DecorationMesh_%d"), DecorationMeshes.Num());
    UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    NewMesh->SetupAttachment(RootComponent);
    NewMesh->SetStaticMesh(Mesh);
    NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NewMesh->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE));

    DecorationMeshes.Add(NewMesh);
}

void AMapMeshWithMutlipleStaticMesh::AddWalkableMesh(UStaticMesh* Mesh)
{
    FString Name = FString::Printf(TEXT("WalkableMesh_%d"), WalkableMeshes.Num());
    UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    NewMesh->SetupAttachment(RootComponent);
    NewMesh->SetStaticMesh(Mesh);
    NewMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    NewMesh->SetCollisionObjectType(ECC_WorldStatic);
    NewMesh->SetCollisionResponseToAllChannels(ECR_Block);
    NewMesh->SetCanEverAffectNavigation(true);
    NewMesh->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE));
    WalkableMeshes.Add(NewMesh);

}

void AMapMeshWithMutlipleStaticMesh::AddBlockPlayerOnlyMesh(UStaticMesh* Mesh)
{
    FString Name = FString::Printf(TEXT("BlockPlayerOnlyMesh_%d"), BlockPlayerOnlyMeshes.Num());
    UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    NewMesh->SetupAttachment(RootComponent);
    NewMesh->SetStaticMesh(Mesh);
    NewMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    NewMesh->SetCollisionObjectType(ECC_WorldStatic);
    NewMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    NewMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    NewMesh->SetCanEverAffectNavigation(false);
    NewMesh->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE));

    BlockPlayerOnlyMeshes.Add(NewMesh);

}

