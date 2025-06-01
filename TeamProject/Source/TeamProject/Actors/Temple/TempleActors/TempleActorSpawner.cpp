#include "TempleActorSpawner.h"
#include "TempleActor.h"

// Sets default values
ATempleActorSpawner::ATempleActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TempleActorClass = ATempleActor::StaticClass();
}

// Called when the game starts or when spawned
void ATempleActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (TempleActorClass)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ATempleActorSpawner::SpawnActor,
			SpawnInterval,
			true
		);
	}
}

void ATempleActorSpawner::SpawnActor()
{
	if (!TempleActorClass) return;

	FVector Origin = GetActorLocation();
	FVector RandomOffset = FVector(
		FMath::RandRange(-SpawnAreaExtent.X, SpawnAreaExtent.X),
		FMath::RandRange(-SpawnAreaExtent.Y, SpawnAreaExtent.Y),
		FMath::RandRange(-SpawnAreaExtent.Z, SpawnAreaExtent.Z) 
	);
	FVector SpawnLocation = Origin + RandomOffset;

	FRotator SpawnRotation = FRotator::ZeroRotator;

	/*ATempleActor* SpawnedActor = GetWorld()->SpawnActor<ATempleActor>(
		TempleActorClass,
		SpawnLocation,
		SpawnRotation
	);

	if (SpawnedActor)
	{
		SpawnedActor->SetData(SpawnRowHandle);
	}*/

	ATempleActor* Actor = GetPooledActor();
	if (Actor)
	{
		Actor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);
		Actor->ActivateActor();
	}
}

void ATempleActorSpawner::ReturnActorToPool(ATempleActor* Actor)
{
	if (!Actor) return;

	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
	Actor->DeactivateActor();
}

TObjectPtr<ATempleActor> ATempleActorSpawner::GetPooledActor()
{
	for (ATempleActor* Actor : ActorPool)
	{
		if (!Actor->IsActive())
		{
			return Actor;
		}
	}

	FActorSpawnParameters SpawnParams;
	ATempleActor* NewActor = GetWorld()->SpawnActor<ATempleActor>(TempleActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (NewActor)
	{
		NewActor->SetData(SpawnRowHandle);
		NewActor->Initialize(this);
		ActorPool.Add(NewActor);
	}
	return NewActor;
}
