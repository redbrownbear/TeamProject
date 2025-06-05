// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

}


void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::SetStaticMesh(UStaticMesh* InMesh)
{
	StaticMeshComponent->SetStaticMesh(InMesh);
}



void AWeaponBase::LeftClickAction()
{

}

void AWeaponBase::RightClickAction()
{
}

