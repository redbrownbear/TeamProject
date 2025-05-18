#include "FurikoController.h"
#include "Components/FSM/Npc/Furiko/FurikoFSMComponent.h"
#include "Character/Npc/Furiko/Furiko.h"

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

    /*if (GetPawn())
    {
        FurikoFSMComponent->SetOwner(Cast<AFuriko>(GetPawn()));
    }*/
}

void AFurikoController::Hide()
{
    // @TODO
}
