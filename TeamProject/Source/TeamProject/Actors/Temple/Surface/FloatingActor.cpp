#include "Actors/Temple/Surface/FloatingActor.h"
#include "FlowSurface.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Data/TempleActorTableRow.h"
#include "Misc/Utils.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(
			TEXT("/Game/Temple/Floating/PM_Floating.PM_Floating")
		);
		check(PhysMaterial.Object);
		PhysicalMaterial = PhysMaterial.Object;

	}

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Floating);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetVisibility(true); 	
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();	

	SetData(DataTableRowHandle);

	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetMassOverrideInKg(NAME_None, FLOATINGACTOR_MASS_KG, true);
	CollisionComponent->SetLinearDamping(FLOATINGACTOR_LINEAR_DAMPING);

	if (PhysicalMaterial)
	{
		CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
	}

	FloatingSpeed = FLOWSURFACE_MOVING_SPEED;

}

void AFloatingActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetData(DataTableRowHandle);

	if (FloatingActorData)
	{
		if (StaticMeshComponent && FloatingActorData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(FloatingActorData->StaticMesh);
		}
	}
}

void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloatingActor::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }

	FFloatingActorTableRow* Data = DataTableRowHandle.GetRow<FFloatingActorTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	FloatingActorData = Data;
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Floating); 
		CollisionComponent->RegisterComponent();
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->SetSimulatePhysics(true);
	}

	if (StaticMeshComponent)
	{
		if (FloatingActorData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(FloatingActorData->StaticMesh);
		}


		//StaticMeshComponent->SetRelativeTransform(FloatingActorData->MeshTransform);
		StaticMeshComponent->SetRelativeScale3D(FloatingActorData->MeshTransform.GetScale3D());
		//CollisionComponent->SetSimulatePhysics(false);
	}
}

