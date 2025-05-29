#include "Actors/Temple/Surface/FlowSurface.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

#include "Misc/Utils.h"

#define FLOWSURFACE_FLOOR_NUM				10
#define FLOWSURFACE_DEFAULT_BOX_EXTENT		FVector(128.0, 48.0, 1.0)
#define FLOWSURFACE_MOVING_SPEED			60.f

AFlowSurface::AFlowSurface()
{
 	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		//static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Script/PhysicsCore.PhysicalMaterial'/Game/Blueprint/NPC/ConveyorBelt/PM_ConveyorBelt.PM_ConveyorBelt'"));
		//check(PhysMaterial.Object);
		//PhysicalMaterial = PhysMaterial.Object;
	}
	
	FlowSpeed = FLOWSURFACE_MOVING_SPEED;
	CollisionComponent_Array.Reserve(FLOWSURFACE_FLOOR_NUM);
	fDistanceAlongSpline_Array.Reserve(FLOWSURFACE_FLOOR_NUM);

	for (int32 i = 0; i < FLOWSURFACE_FLOOR_NUM; ++i)
	{
		FString ComponentName = FString::Printf(TEXT("CollisionComponent_%d"), i);
		CollisionComponent_Array.Push(CreateDefaultSubobject<UBoxComponent>(*ComponentName));
		CollisionComponent_Array[i]->SetBoxExtent(FLOWSURFACE_DEFAULT_BOX_EXTENT);
		CollisionComponent_Array[i]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		//if (!HasAnyFlags(RF_ClassDefaultObject))
		//{
		//	CollisionComponent_Array[i]->SetPhysMaterialOverride(PhysicalMaterial);
		//}
		CollisionComponent_Array[i]->SetCollisionProfileName(CollisionProfileName::MapMesh);
		fDistanceAlongSpline_Array.Push(0.f);
	}

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetClosedLoop(true);
	SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AFlowSurface::BeginPlay()
{
	Super::BeginPlay();

	float SplineLength = SplineComponent->GetSplineLength();

	for (int32 i = 0; i < FLOWSURFACE_FLOOR_NUM; ++i)
	{
		fDistanceAlongSpline_Array[i] = SplineLength / FLOWSURFACE_FLOOR_NUM * i;
		//CollisionComponent_Array[i]->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	}
	bMove = true;

	FlowSpeed = FLOWSURFACE_MOVING_SPEED;
}

// Called every frame
void AFlowSurface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMove)
	{
		for (int i = 0; i < FLOWSURFACE_FLOOR_NUM; ++i)
		{
			fDistanceAlongSpline_Array[i] += DeltaTime * FlowSpeed;

			// 스플라인의 길이를 초과하지 않도록 처리
			if (fDistanceAlongSpline_Array[i] > SplineComponent->GetSplineLength())
			{
				fDistanceAlongSpline_Array[i] = 0.f; // 다시 처음으로
			}
			FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(fDistanceAlongSpline_Array[i], ESplineCoordinateSpace::World);

			CollisionComponent_Array[i]->SetWorldLocation(NewLocation);
		}
	}
}


