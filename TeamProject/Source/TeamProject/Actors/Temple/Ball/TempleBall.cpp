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
	//CollisionComponent->InitSphereRadius(5000.0f);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/Ball/PM_TempleBall.PM_TempleBall"));
	PhysicalMaterial = PhysMaterial.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Temple/Ball/DgnObj_AncientBallL_01.DgnObj_AncientBallL_01"));
	StaticMeshComponent->SetStaticMesh(BallMesh.Object);
	StaticMeshComponent->SetWorldScale3D(FVector(100.f, 100.f, 100.f));

}

// Called when the game starts or when spawned
void ATempleBall::BeginPlay()
{
	Super::BeginPlay();	

	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);

	/*StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);*/

	StaticMeshComponent->BodyInstance.bUseCCD = true;
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);

	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);
}
