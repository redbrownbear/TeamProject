#include "FurikoController.h"
#include "Component/Npc/Furiko/FurikoFSMComponent.h"

AFurikoController::AFurikoController()
    : ANpcController()
{
	NpcFSMComponent = CreateDefaultSubobject<UFurikoFSMComponent>(TEXT("NpcFSMComponent"));
}

void AFurikoController::BeginPlay()
{
    Super::BeginPlay();

    if (GetPawn())
    {
        NpcFSMComponent = GetPawn()->FindComponentByClass<UFurikoFSMComponent>();
    }
}
