#include "Actors/Object/MetalActor.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Data/MetalActorTableRow.h"

// Sets default values
AMetalActor::AMetalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/MetalActors/PM_MetalActor.PM_MetalActor"));
	PhysicalMaterial = PhysMaterial.Object;
}

// Called when the game starts or when spawned
void AMetalActor::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetGenerateOverlapEvents(true);

	StaticMeshComponent->BodyInstance.bUseCCD = true;
	StaticMeshComponent->SetEnableGravity(true);

	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);

	SetData(DataTableRowHandle);
}

void AMetalActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetData(DataTableRowHandle);
}

void AMetalActor::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }

	FMetalActorTableRow* Data = DataTableRowHandle.GetRow<FMetalActorTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MetalActorData = Data;
	if (CollisionComponent && MetalActorData)
	{
		CollisionComponent->SetCollisionProfileName(MetalActorData->CollisionProfileName);
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->SetSimulatePhysics(true);
		CollisionComponent->SetMassOverrideInKg(NAME_None, MetalActorData->MassInKg, true);
		CollisionComponent->SetLinearDamping(MetalActorData->LinearDamping);
	}

	if (StaticMeshComponent)
	{
		if (MetalActorData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(MetalActorData->StaticMesh);
		}


		StaticMeshComponent->SetRelativeScale3D(MetalActorData->MeshTransform.GetScale3D());
	}
}

