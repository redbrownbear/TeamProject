#include "NpcController.h"
#include "Character/Npc/Npc.h"
#include "Components/FSM/Npc/NpcFSMComponent.h"

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
    }        
}
