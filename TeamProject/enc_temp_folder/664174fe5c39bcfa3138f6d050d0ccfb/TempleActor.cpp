#include "TempleActor.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ActorDeleteVolume.h"

#include "Data/TempleActorTableRow.h"

// Sets default values
ATempleActor::ATempleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	//CollisionComponent->InitSphereRadius(5000.0f);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/TempleActors/PM_TempleActor.PM_TempleActor"));
	PhysicalMaterial = PhysMaterial.Object;
}

// Called when the game starts or when spawned
void ATempleActor::BeginPlay()
{
	Super::BeginPlay();	

	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);

	StaticMeshComponent->BodyInstance.bUseCCD = true;
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);

	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);
}

void ATempleActor::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }

	FTempleActorTableRow* Data = DataTableRowHandle.GetRow<FTempleActorTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	TempleActorData = Data;
	if (CollisionComponent)
	{
		//CollisionComponent->SetCollisionProfileName(CollisionProfileName::Floating);
		CollisionComponent->RegisterComponent();
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->SetSimulatePhysics(true);
	}

	if (StaticMeshComponent)
	{
		if (TempleActorData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(TempleActorData->StaticMesh);
		}


		//StaticMeshComponent->SetRelativeTransform(FloatingActorData->MeshTransform);
		StaticMeshComponent->SetRelativeScale3D(TempleActorData->MeshTransform.GetScale3D());
		//CollisionComponent->SetSimulatePhysics(false);
	}
}
