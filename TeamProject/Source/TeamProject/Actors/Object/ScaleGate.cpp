// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/ScaleGate.h"
#include "SubSystem/Puzzle/EventManager.h"

// Sets default values
AScaleGate::AScaleGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetRelativeScale3D(FVector(90.f, 90.f, 90.f));
}

// Called when the game starts or when spawned
void AScaleGate::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}

	UEventManager* EventManager = GetGameInstance()->GetSubsystem<UEventManager>();
	if (EventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventManager found, adding dynamic delegate"));

		EventManager->OnWeightFull.AddDynamic(this, &AScaleGate::OpenScaleGate);
	}
}

void AScaleGate::OpenScaleGate()
{
	StartLocation = StaticMeshComponent->GetComponentLocation();
	TargetLocation = StartLocation + FVector::UpVector * GetComponentsBoundingBox().GetSize().Z;
	ElapsedTime = 0.f;

	GetWorldTimerManager().SetTimer(GateMoveTimer, this, &AScaleGate::MoveGateTick, 0.01f, true);
}

void AScaleGate::MoveGateTick()
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
