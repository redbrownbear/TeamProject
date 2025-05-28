// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/ThrownObject.h"
#include "Actors/Monster/MonsterInterface.h"
#include "Data/ItemDataRow.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AThrownObject::AThrownObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AThrownObject::SetDataWithHandle(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FItemData* Data = DataTableRowHandle.GetRow<FItemData>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	ItemTableRow = Data;

	// StaticMesh
	if (ItemTableRow->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(ItemTableRow->StaticMesh);
		StaticMeshComponent->SetRelativeTransform(ItemTableRow->Transform);
	}
}

// Called when the game starts or when spawned
void AThrownObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrownObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrownObject::AttachToMonster(IMonsterInterface* Monster, FName SocketName)
{
	if (!Monster) { return; }

	if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
	{
		// Offset Changed to fix outlook
		StaticMeshComponent->SetRelativeLocation(FVector::Zero());
		const bool bSucceeded = this->AttachToComponent(
			Monster->GetMonsterMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName);

		if (!bSucceeded)
		{
			check(false);
		}
	}
}

