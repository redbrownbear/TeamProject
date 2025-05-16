#include "NpcController.h"
#include "Npc.h"
#include "NpcFSMComponent.h"

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
