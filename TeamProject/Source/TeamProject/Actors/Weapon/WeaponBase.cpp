// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

}

void AWeaponBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FWeaponTableRow* Data = DataTableRowHandle.GetRow<FWeaponTableRow>(TEXT("Weapon"));
	if (!Data) { ensure(false); return; }

	SkeletalMeshComponent->SetSkeletalMesh(Data->SkeletalMesh);
	SkeletalMeshComponent->SetRelativeTransform(Data->Transform);
	check(Data->AnimClass);
	USkeletalMeshComponent* MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	check(MeshComponent);
	MeshComponent->SetAnimClass(Data->AnimClass);
}

void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
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

void AWeaponBase::LeftClickAction()
{

}

void AWeaponBase::RightClickAction(bool _bool)
{
}

