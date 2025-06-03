#include "Actors/Temple/TempleActors/TempleSwitch.h"
#include "Components/SphereComponent.h"
#include "TempleActor.h"
#include "LockedGate.h"
#include "TempleActorSpawner.h"

#include "Data/TempleActorTableRow.h"

// Sets default values
ATempleSwitch::ATempleSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATempleSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATempleSwitch::OnBeginOverlapWithBall);
	}
}

void ATempleSwitch::UnlockGate()
{
	if (LockedGate)
	{
		LockedGate->OpenGateSequence();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LockedGate is null!"));
	}
}

void ATempleSwitch::OnBeginOverlapWithBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	ATempleActor* OverlappedTempleActor = Cast<ATempleActor>(OtherActor);
	if (!OverlappedTempleActor) return;

	const FTempleActorTableRow* Data = OverlappedTempleActor->GetTempleActorData();
	if (!Data) return;

	if (Data->ActorName.Equals(TEXT("Key_Ball")))
	{
		UnlockGate(); 
		bIsOpenedGate = true;
	}
}


