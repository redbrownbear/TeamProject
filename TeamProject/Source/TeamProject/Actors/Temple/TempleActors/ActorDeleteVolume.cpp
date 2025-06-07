#include "ActorDeleteVolume.h"
#include "TempleActor.h"
#include "TempleActorSpawner.h"

void AActorDeleteVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AActorDeleteVolume::OnOverlapBegin);
}

void AActorDeleteVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	/*if (ATempleActor* Actor = Cast<ATempleActor>(OtherActor))
	{
		Actor->Destroy();
	}*/

	if (ATempleActor* Actor = Cast<ATempleActor>(OtherActor))
	{
		if (Actor->IsActive())
		{
			if (Actor->GetWorld() && Actor->GetWorld()->IsGameWorld())
			{
				if (Actor->GetTempleActorSpawner())
				{
					Actor->GetTempleActorSpawner()->ReturnActorToPool(Actor);
				}
			}
		}
	}
}
