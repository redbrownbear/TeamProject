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
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetGenerateOverlapEvents(true);

	StaticMeshComponent->BodyInstance.bUseCCD = true;
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
	if (CollisionComponent && TempleActorData)
	{
		CollisionComponent->SetCollisionProfileName(TempleActorData->CollisionProfileName);
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->SetSimulatePhysics(true);
		CollisionComponent->SetMassOverrideInKg(NAME_None, TempleActorData->MassInKg, true);
		CollisionComponent->SetLinearDamping(TempleActorData->LinearDamping);
	}

	if (StaticMeshComponent)
	{
		if (TempleActorData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(TempleActorData->StaticMesh);
		}


		StaticMeshComponent->SetRelativeScale3D(TempleActorData->MeshTransform.GetScale3D());		
	}
}
