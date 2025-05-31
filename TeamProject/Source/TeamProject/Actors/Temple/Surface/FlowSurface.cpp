#include "Actors/Temple/Surface/FlowSurface.h"
#include "FloatingActor.h"
#include "Components/BoxComponent.h"
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

	CollisionComponent_Array.Reserve(FLOWSURFACE_FLOOR_NUM); // Reserve는 여기서 해도 무방합니다.
	fDistanceAlongSpline_Array.Reserve(FLOWSURFACE_FLOOR_NUM); // Reserve는 여기서 해도 무방합니다.

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

			// 실제 부착 수행
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

			// 스플라인의 길이를 초과하지 않도록 처리
			if (fDistanceAlongSpline_Array[i] > SplineComponent->GetSplineLength())
			{
				fDistanceAlongSpline_Array[i] = 0.f; // 다시 처음으로
			}
			FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(fDistanceAlongSpline_Array[i], ESplineCoordinateSpace::World);

			CollisionComponent_Array[i]->SetWorldLocation(NewLocation);

			// FloatingActor가 Spline 따라 흘러가도록
			TArray<AActor*> OverlappingActors;
			CollisionComponent_Array[i]->GetOverlappingActors(OverlappingActors, AFloatingActor::StaticClass());

			for (AActor* Actor : OverlappingActors)
			{
				AFloatingActor* Floating = Cast<AFloatingActor>(Actor);
				if (Floating && Floating->GetCollisionComponent())
				{
					// 흐름 방향 계산
					FVector FlowDirection = SplineComponent->GetDirectionAtDistanceAlongSpline(
						fDistanceAlongSpline_Array[i], ESplineCoordinateSpace::World);
					FlowDirection.Z = 0.f;

					FVector FlowForce = FlowDirection.GetSafeNormal() * FLOATINGACTOR_FORCE; // 힘 세기 조절
					Floating->GetCollisionComponent()->AddForce(FlowForce, NAME_None, true);
				}
			}
		}

	}
}


