#include "ActorDeleteVolume.h"
#include "TempleActor.h"

void AActorDeleteVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AActorDeleteVolume::OnOverlapBegin);
}

void AActorDeleteVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ATempleActor* Actor = Cast<ATempleActor>(OtherActor))
	{
		Actor->Destroy();
	}
}
