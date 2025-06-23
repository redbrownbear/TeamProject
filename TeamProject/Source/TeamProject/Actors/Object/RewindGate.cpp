// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/RewindGate.h"

// Sets default values
ARewindGate::ARewindGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset
	{ TEXT("/Script/Engine.StaticMesh'/Game/Resources/Object/Door/DgnObj_IronDoorR_B_01.DgnObj_IronDoorR_B_01'") };
	if (Asset.Object)
	{
		StaticMeshComponent->SetStaticMesh(Asset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARewindGate::ARewindGate // No ARewindGate StaticMeshAsset"));
	}

	StaticMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	StaticMeshComponent->SetRelativeScale3D(FVector(90.f, 90.f, 90.f));
}

// Called when the game starts or when spawned
void ARewindGate::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}

	if(!bIsOpen) OpenFinalGate();
}

void ARewindGate::OpenFinalGate()
{
	if (bIsOpen || GetWorldTimerManager().IsTimerActive(GateMoveTimer)) return;
	GetWorldTimerManager().ClearTimer(GateMoveTimer);

	StartLocation = StaticMeshComponent->GetComponentLocation();
	TargetLocation = StartLocation + FVector::UpVector * GetComponentsBoundingBox().GetSize().Z;
	ElapsedTime = 0.f;

	GetWorldTimerManager().SetTimer(GateMoveTimer, this, &ARewindGate::MoveGateTick, 0.01f, true);
	bIsOpen = true;
}

void ARewindGate::CloseFinalGate()
{
	if (!bIsOpen || GetWorldTimerManager().IsTimerActive(GateMoveTimer)) return;

	GetWorldTimerManager().ClearTimer(GateMoveTimer);

	StartLocation = StaticMeshComponent->GetComponentLocation();
	TargetLocation = StartLocation + FVector::DownVector * GetComponentsBoundingBox().GetSize().Z;
	ElapsedTime = 0.f;

	GetWorldTimerManager().SetTimer(GateMoveTimer, this, &ARewindGate::MoveGateTick, 0.01f, true);

	bIsOpen = false;
}

void ARewindGate::MoveGateTick()
{
	ElapsedTime += 0.01f;

	float Alpha = ElapsedTime / Duration;
	if (Alpha >= 1.f)
	{
		StaticMeshComponent->SetWorldLocation(TargetLocation);
		GetWorldTimerManager().ClearTimer(GateMoveTimer);
		return;
	}

	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	StaticMeshComponent->SetWorldLocation(NewLocation);
}
