#include "TempleActorSpawner.h"
#include "TempleActor.h"
#include "Misc/Defines.h"

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

	float SpawnInterval;

	SpawnInterval = DUNGEON_BALL_SPAWNINTERVAL;
	SpawnAreaExtent = DUNGEON_BALL_SPAWNAREAEXTENT;
	
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

	//GetWorld()->SpawnActor<ATempleActor>(TempleActorClass, SpawnLocation, SpawnRotation);

	ATempleActor* SpawnedActor = GetWorld()->SpawnActor<ATempleActor>(
		TempleActorClass,
		SpawnLocation,
		SpawnRotation
	);

	if (SpawnedActor)
	{
		SpawnedActor->SetData(SpawnRowHandle);
	}
}
