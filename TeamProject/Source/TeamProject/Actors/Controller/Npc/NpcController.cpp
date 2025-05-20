#include "NpcController.h"
#include "Actors/Npc/Npc.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "Actors/Character/PlayerCharacter.h"

ANpcController::ANpcController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANpcController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANpcController::BeginPlay()
{
	Super::BeginPlay();

	if (NpcFSMComponent)
	{
		NpcFSMComponent->SetOwner(Cast<ANpc>(GetPawn()));
		NpcFSMComponent->SetPlayer(nullptr);
	}
}
