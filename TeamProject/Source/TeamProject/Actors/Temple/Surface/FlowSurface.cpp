#include "Actors/Temple/Surface/FlowSurface.h"
#include "Actors/Temple/TempleActors/TempleActor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"

#include "Misc/Utils.h"

AFlowSurface::AFlowSurface()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetClosedLoop(true);
	SplineComponent->SetupAttachment(RootComponent);

	CollisionComponent_Array.Reserve(FLOWSURFACE_FLOOR_NUM); 
	fDistanceAlongSpline_Array.Reserve(FLOWSURFACE_FLOOR_NUM); 

	FlowSpeed = FLOWSURFACE_MOVING_SPEED;
}

// Called when the game starts or when spawned
void AFlowSurface::BeginPlay()
{
	Super::BeginPlay();

	bMove = true;

	FlowSpeed = FLOWSURFACE_MOVING_SPEED;

	const float SplineLength = SplineComponent->GetSplineLength();
	for (int32 i = 0; i < FLOWSURFACE_FLOOR_NUM; ++i)
	{
		const float DevidedLength = SplineLength / FLOWSURFACE_FLOOR_NUM * i;
		fDistanceAlongSpline_Array.Push(DevidedLength);

		FString ComponentName = FString::Printf(TEXT("CollisionComponent%d"), i);
		UBoxComponent* NewBoxComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), FName(*ComponentName));

		if (NewBoxComponent)
		{
			NewBoxComponent->SetBoxExtent(FLOWSURFACE_DEFAULT_BOX_EXTENT);
			NewBoxComponent->RegisterComponent();

			CollisionComponent_Array.Add(NewBoxComponent);

			NewBoxComponent->SetCanEverAffectNavigation(false);
			NewBoxComponent->SetCollisionProfileName(CollisionProfileName::Water); 

			FAttachmentTransformRules AttachRules = FAttachmentTransformRules::KeepRelativeTransform;

			NewBoxComponent->AttachToComponent(RootComponent, AttachRules);
			NewBoxComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		}
	}
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

			// ���ö����� ���̸� �ʰ����� �ʵ��� ó��
			if (fDistanceAlongSpline_Array[i] > SplineComponent->GetSplineLength())
			{
				fDistanceAlongSpline_Array[i] = 0.f; // �ٽ� ó������
			}
			FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(fDistanceAlongSpline_Array[i], ESplineCoordinateSpace::World);

			CollisionComponent_Array[i]->SetWorldLocation(NewLocation);

			// Floating Actors
			TArray<AActor*> OverlappingActors;
			CollisionComponent_Array[i]->GetOverlappingActors(OverlappingActors, ATempleActor::StaticClass());

			for (AActor* Actor : OverlappingActors)
			{
				ATempleActor* Floating = Cast<ATempleActor>(Actor);
				if (Floating && Floating->GetCollisionComponent())
				{
					FVector FlowDirection = SplineComponent->GetDirectionAtDistanceAlongSpline(
						fDistanceAlongSpline_Array[i], ESplineCoordinateSpace::World);
					FlowDirection.Z = 0.f;

					FVector FlowForce = FlowDirection.GetSafeNormal() * FLOATINGACTOR_FORCE; 
					Floating->GetCollisionComponent()->AddForce(FlowForce, NAME_None, true);
				}
			}
		}

	}
}


