#include "FurikoController.h"
#include "Components/FSMComponent/Npc/Furiko/FurikoFSMComponent.h"

AFurikoController::AFurikoController()
    : ANpcController()
{	
    NpcFSMComponent = CreateDefaultSubobject<UFurikoFSMComponent>(TEXT("FurikoFSMComponent"));
    
}

void AFurikoController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);    
   
}

void AFurikoController::BeginPlay()
{
    Super::BeginPlay();

}

