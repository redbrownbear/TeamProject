#include "Actors/Temple/Ball/BallTriggerVolume.h"
#include "TempleBall.h"

void ABallTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABallTriggerVolume::OnOverlapBegin);
}

void ABallTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ATempleBall* Ball = Cast<ATempleBall>(OtherActor))
	{
		Ball->Destroy(); 
	}
}
