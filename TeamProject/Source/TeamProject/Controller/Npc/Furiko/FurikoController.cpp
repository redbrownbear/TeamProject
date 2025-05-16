#include "FurikoController.h"
#include "Components/Npc/Furiko/FurikoFSMComponent.h"
#include "Character/Npc/Npc.h"

AFurikoController::AFurikoController()
    : ANpcController()
{	
    FurikoFSMComponent = CreateDefaultSubobject<UFurikoFSMComponent>(TEXT("FurikoFSMComponent"));

    // 맵에 있는 StrollPath 자동 탐색
    /*for (TActorIterator<AStrollPath> It(GetWorld()); It; ++It)
    {
        StrollPathActor = *It;
        break;
    }*/
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
    // @TODO 랜덤한 장소로 이동
}
