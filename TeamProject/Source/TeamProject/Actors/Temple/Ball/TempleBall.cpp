#include "Actors/Temple/Ball/TempleBall.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "BallTriggerVolume.h"

// Sets default values
ATempleBall::ATempleBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));		

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/Ball/BP_TempleBall.BP_TempleBall"));
	PhysicalMaterial = PhysMaterial.Object;
}

// Called when the game starts or when spawned
void ATempleBall::BeginPlay()
{
	Super::BeginPlay();	

	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);

	StaticMeshComponent->BodyInstance.bUseCCD = true;
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);
}
