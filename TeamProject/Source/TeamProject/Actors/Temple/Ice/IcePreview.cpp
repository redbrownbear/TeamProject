#include "Actors/Temple/Ice/IcePreview.h"
#include "Components/TimelineComponent.h"

// Sets default values
AIcePreview::AIcePreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	PreviewTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PreviewTimeline"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/Resources/Map/Dungeon/DgnObj_Ice.DgnObj_Ice"));
	StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
}

// Called when the game starts or when spawned
void AIcePreview::BeginPlay()
{
	Super::BeginPlay();

	if (RiseCurve)
	{
		FOnTimelineFloat Progress;
		Progress.BindUFunction(this, FName("StartPreview"));
		PreviewTimeline->AddInterpFloat(RiseCurve, Progress);
		PreviewTimeline->PlayFromStart();
	}

	MaterialInterface = StaticMeshComponent->GetMaterial(0);
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	DynamicMaterialInstance->SetScalarParameterValue("Alpha", 0.0f);

}

// Called every frame
void AIcePreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIcePreview::StartPreview()
{
	// Play Growing Ice Montage
}

void AIcePreview::StopPreview()
{
	// Delete Growing Ice Montage
	bIsAnimating = false;
}

void AIcePreview::UpdateLocation(const FVector& WorldLocation)
{

}

