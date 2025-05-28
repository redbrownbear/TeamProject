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
}

// Called when the game starts or when spawned
void AIcePreview::BeginPlay()
{
	Super::BeginPlay();

	MaterialInterface = StaticMeshComponent->GetMaterial(0);
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInstance);
	DynamicMaterialInstance->SetScalarParameterValue("Alpha", 0.1f);

}

// Called every frame
void AIcePreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("IcePreview Z: %f"), Location.Z);

	if (bIsRising)
	{
		float DeltaZ = RiseSpeed * DeltaTime;
		CurrentRise += DeltaZ;

		if (CurrentRise >= RiseDistance)
		{
			DeltaZ -= (CurrentRise - RiseDistance); // 초과분 제거
			bIsRising = false;
		}

		FVector NewLocation = GetActorLocation() + FVector(0.f, 0.f, DeltaZ);
		SetActorLocation(NewLocation);
	}
}

void AIcePreview::StartPreview()
{
	if (bIsRising) return; 

	StartLocation = GetActorLocation() - FVector(0.f, 0.f, RiseDistance);
	SetActorLocation(StartLocation);
	CurrentRise = 0.f;
	bIsRising = true;
}

void AIcePreview::StopPreview()
{
	bIsRising = false;
}


