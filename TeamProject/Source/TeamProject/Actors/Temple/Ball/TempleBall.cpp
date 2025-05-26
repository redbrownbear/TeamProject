#include "Actors/Temple/Ball/TempleBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ATempleBall::ATempleBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/Ball/BP_TempleBall.BP_TempleBall"));
	PhysicalMaterial = PhysMaterial.Object;
}

// Called when the game starts or when spawned
void ATempleBall::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetEnableGravity(true);
	CollisionComponent->SetSimulatePhysics(true);
}

void ATempleBall::Tick(float DeltaTime)
{
	// 특정 지점을 지나면 삭제
	if (FVector::Dist(GetActorLocation(), Cliff) < Distance)
	{
		Destroy();
	}
}


