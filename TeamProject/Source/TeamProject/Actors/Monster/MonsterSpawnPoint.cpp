// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "Data/MonsterTableRow.h"

#include "Actors/Object/CampFire.h"
#include "Actors/Object/PatrolPath.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneComponent;


	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	FRotator NewRotator = FRotator(0.0, 0.0, 0.0);
	SkeletalMeshComponent->SetWorldRotation(NewRotator.Quaternion());
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ... 기존 코드 ...
	if (!SpawnPointGuid.IsValid())
	{
		SpawnPointGuid = FGuid::NewGuid();
	}
}

void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
}

void AMonsterSpawnPoint::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMonsterTableRow* Data = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MonsterData = Data;

	SkeletalMeshComponent->SetSkeletalMesh(MonsterData->SkeletalMesh);
	SkeletalMeshComponent->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AMonsterSpawnPoint::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
		SpawnPointGuid = FGuid::NewGuid();
	}
}

void AMonsterSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!SpawnPointGuid.IsValid())
	{
		SpawnPointGuid = FGuid::NewGuid();
	}

	SetData(DataTableRowHandle);
	SetActorTransform(Transform);

	if (CampFire)
	{
		CampFirePointGuid = CampFire->GetCampFireGuid();
	}
	if (PatrolPath)
	{
		PatrolPathPointGuid = PatrolPath->GetPatrolPathGuid();
	}
}


