#include "Actors/Temple/TempleActors/LockedGate.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ALockedGate::ALockedGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALockedGate::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}		
}

void ALockedGate::OpenGate()
{
	StartLocation = StaticMeshComponent->GetComponentLocation();
	TargetLocation = StartLocation + FVector::UpVector * GetComponentsBoundingBox().GetSize().Z;
	ElapsedTime = 0.f;

	GetWorldTimerManager().SetTimer(GateMoveTimer, this, &ALockedGate::MoveGateTick, 0.01f, true);
}

void ALockedGate::ClearDungeon()
{
	// 클리어 되었다는 시퀀스 재생
}

void ALockedGate::MoveGateTick()
{
	ElapsedTime += 0.01f;

	float Alpha = ElapsedTime / Duration;
	if (Alpha >= 1.f)
	{
		StaticMeshComponent->SetWorldLocation(TargetLocation);
		GetWorldTimerManager().ClearTimer(GateMoveTimer);
		return;
	}

	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	StaticMeshComponent->SetWorldLocation(NewLocation);
}

void ALockedGate::OpenGateSequence()
{
	// 문 열리는 시퀀스 재생 

	OpenGate();
}
