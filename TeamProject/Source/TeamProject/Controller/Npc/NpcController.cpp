#include "NpcController.h"
#include "Character/Npc/Npc.h"
#include "Component/Npc/NpcFSMComponent.h"

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
}
