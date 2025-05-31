#include "ActorDeleteVolume.h"
#include "TempleActor.h"

void ActorDeleteVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ActorDeleteVolume::OnOverlapBegin);
}

void ActorDeleteVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ATempleActor* Ball = Cast<ATempleActor>(OtherActor))
	{
		Ball->Destroy(); 
	}
}
