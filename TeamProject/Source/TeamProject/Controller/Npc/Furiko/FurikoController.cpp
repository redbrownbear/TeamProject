#include "FurikoController.h"
#include "Components/FSM/Npc/Furiko/FurikoFSMComponent.h"
#include "Character/Npc/Npc.h"

AFurikoController::AFurikoController()
    : ANpcController()
{	
    FurikoFSMComponent = CreateDefaultSubobject<UFurikoFSMComponent>(TEXT("FurikoFSMComponent"));
    
}

void AFurikoController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);    

    
}

void AFurikoController::BeginPlay()
{
    Super::BeginPlay();

    if (GetPawn())
    {
        FurikoFSMComponent = GetPawn()->FindComponentByClass<UFurikoFSMComponent>();
    }

}

void AFurikoController::Hide()
{
    // @TODO
}
