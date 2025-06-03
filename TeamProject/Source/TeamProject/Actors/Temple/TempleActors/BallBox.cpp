#include "Actors/Temple/TempleActors/BallBox.h"
#include "Components/SphereComponent.h"
#include "TempleSwitch.h"
#include "TempleActor.h"
#include "TempleActorSpawner.h"

#include "Data/TempleActorTableRow.h"

// Sets default values
ABallBox::ABallBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABallBox::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	GetWorldTimerManager().SetTimer(
		SpawnDelayTimer,
		this,
		&ABallBox::CheckAndSpawnTempleActor,
		5.0f,
		false
	);
}

void ABallBox::CheckAndSpawnTempleActor()
{
	if (bHasSpawnedOnce) return;
	if (!TempleActorSpawner) return;
	if (TempleSwitch->GetGateOpened()) return; 
	
	// Switch 위에 TempleActor가 없을 경우에만
	TArray<AActor*> OverlappingActors;
	CollisionComponent->GetOverlappingActors(OverlappingActors, ATempleActor::StaticClass());
	
	if (OverlappingActors.Num() == 0)
	{
		TempleActorSpawner->SpawnActor();
		bHasSpawnedOnce = true;
	}

}

