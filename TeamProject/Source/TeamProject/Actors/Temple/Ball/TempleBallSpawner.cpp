#include "Actors/Temple/Ball/TempleBallSpawner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATempleBallSpawner::ATempleBallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATempleBallSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATempleBallSpawner::SpawnBall, SpawnInterval, true);
}

void ATempleBallSpawner::SpawnBall()
{
	if (!BallClass) return;

	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox((SpawnAreaMin + SpawnAreaMax) / 2, (SpawnAreaMax - SpawnAreaMin) / 2);
	GetWorld()->SpawnActor<AActor>(BallClass, RandomLocation, FRotator::ZeroRotator);
}


