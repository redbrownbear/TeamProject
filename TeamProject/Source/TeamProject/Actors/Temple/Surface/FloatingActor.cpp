#include "Actors/Temple/Surface/FloatingActor.h"
#include "FlowSurface.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Data/FloatingActorTableRow.h"
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

	FloatingSpeed = FLOWSURFACE_MOVING_SPEED;

	/*if (FlowSurface && GetOwner() == nullptr)
	{
		const int32 NumClones = 5;
		const float SplineLength = FlowSurface->GetSplineComponent()->GetSplineLength();

		for (int32 i = 1; i < NumClones; ++i) 
		{
			const float StartDistance = (SplineLength / NumClones) * i;
			const FVector SpawnLocation = FlowSurface->GetSplineComponent()->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);
			const FRotator SpawnRotation = GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			AFloatingActor* Clone = GetWorld()->SpawnActor<AFloatingActor>(GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
			if (Clone)
			{
				Clone->FlowSurface = FlowSurface;
				Clone->DistanceAlongSpline = StartDistance;
				Clone->FloatingSpeed = FloatingSpeed;
				Clone->SetData(DataTableRowHandle);
			}
		}
	}*/
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

	//if (FlowSurface && FlowSurface->GetSplineComponent())
	//{
	//	DistanceAlongSpline += DeltaTime * FloatingSpeed;

	//	float SplineLength = FlowSurface->GetSplineComponent()->GetSplineLength();
	//	if (DistanceAlongSpline > SplineLength)
	//	{
	//		DistanceAlongSpline = 0.f; // 루프되도록
	//	}

	//	FVector NewLocation = FlowSurface->GetSplineComponent()->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	//	SetActorLocation(NewLocation);
	//}

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

