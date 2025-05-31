#include "Actors/Temple/Ball/TempleBallSpawner.h"
#include "TempleBall.h"
#include "Misc/Defines.h"

// Sets default values
ATempleBallSpawner::ATempleBallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BallClass = ATempleBall::StaticClass();
}

// Called when the game starts or when spawned
void ATempleBallSpawner::BeginPlay()
{
	Super::BeginPlay();

	float SpawnInterval;

	SpawnInterval = DUNGEON_BALL_SPAWNINTERVAL;
	SpawnAreaExtent = DUNGEON_BALL_SPAWNAREAEXTENT;
	
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

	//DrawDebugBox(
	//	GetWorld(),
	//	GetActorLocation(),
	//	SpawnAreaExtent,
	//	FColor::Green,
	//	true, // 지속
	//	-1,   // 무제한 시간
	//	0,
	//	2
	//);

}

void ATempleBallSpawner::SpawnBall()
{
	if (!BallClass) return;

	// 랜덤 위치 계산 (스폰 범위 안에서)
	FVector Origin = GetActorLocation();
	FVector RandomOffset = FVector(
		FMath::RandRange(-SpawnAreaExtent.X, SpawnAreaExtent.X),
		FMath::RandRange(-SpawnAreaExtent.Y, SpawnAreaExtent.Y),
		FMath::RandRange(-SpawnAreaExtent.Z, SpawnAreaExtent.Z) // Z도 확장하려면 0.f 이상으로
	);
	FVector SpawnLocation = Origin + RandomOffset;

	FRotator SpawnRotation = FRotator::ZeroRotator;
	GetWorld()->SpawnActor<ATempleBall>(BallClass, SpawnLocation, SpawnRotation);
}
