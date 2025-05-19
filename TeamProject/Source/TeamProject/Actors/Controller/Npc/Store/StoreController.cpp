#include "Actors/Controller/Npc/Store/StoreController.h"
#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"

AStoreController::AStoreController()
	: ANpcController()
{
	NpcFSMComponent = CreateDefaultSubobject<UStoreFSMComponent>(TEXT("StoreFSMComponent"));
}

void AStoreController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStoreController::BeginPlay()
{
	Super::BeginPlay();

}
