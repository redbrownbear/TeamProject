#include "Actors/Temple/Ball/TempleBallSpawner.h"
#include "TempleBall.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATempleBallSpawner::ATempleBallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ATempleBallSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (BallClass)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ATempleBallSpawner::SpawnBall,
			SpawnInterval,
			true
		);
	}
}

void ATempleBallSpawner::SpawnBall()
{
	if (!BallClass) return;

	FVector Location = GetActorLocation();
	FRotator Rotation = FRotator::ZeroRotator;

	GetWorld()->SpawnActor<ATempleBall>(BallClass, Location, Rotation);
}


