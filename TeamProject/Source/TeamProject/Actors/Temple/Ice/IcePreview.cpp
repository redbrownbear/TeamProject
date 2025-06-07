#include "Actors/Temple/Ice/IcePreview.h"
#include "Components/TimelineComponent.h"

// Sets default values
AIcePreview::AIcePreview()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/Resources/Map/Dungeon/DgnObj_Ice.DgnObj_Ice"));
	StaticMeshComponent->SetStaticMesh(StaticMesh.Object);

	UE_LOG(LogTemp, Warning, TEXT("IcePreview Construct"));
}

// Called when the game starts or when spawned
void AIcePreview::BeginPlay()
{
	Super::BeginPlay();

	MaterialInterface = StaticMeshComponent->GetMaterial(0);
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInstance);
	DynamicMaterialInstance->SetScalarParameterValue("Alpha", 0.1f);

	StartLocation = GetActorLocation();
	SetActorLocation(StartLocation - FVector(0, 0, MaxHeight));
	CurrentRise = 0.f;
	bIsRising = true;
}

// Called every frame
void AIcePreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsRising) return;

	if (bCanSpawn)
	{
		const FVector CurrentLocation = GetActorLocation();
		const FVector ReverseDirection = -1 * RiseDirection;
		const FVector FirstLocation = PivotLocation + ReverseDirection * MaxHeight;

		FVector NextLocation = CurrentLocation + RiseDirection * MaxSpeed * DeltaTime;
		const float fDistance = FVector::Dist(CurrentLocation, FirstLocation);

		if (fDistance > MaxHeight)
		{
			SetActorLocation(FirstLocation);
		}
		else
		{
			SetActorLocation(NextLocation);
		}
	}
	else
	{
		SetActorLocation(PivotLocation);
	}
}

void AIcePreview::SetRiseDirection(FVector InDirection)
{
	RiseDirection = InDirection;

}

void AIcePreview::SetPivotLocation(FVector InPosition)
{
	PivotLocation = InPosition;
}


