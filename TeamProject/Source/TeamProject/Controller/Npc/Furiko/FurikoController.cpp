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

void AFurikoController::SetFocus()
{
    // 1. 무작위로 움직이다 플레이어의 콜리전이 오버랩되면 플레이어로 시선 고정
    // 2. 콜리전 오버랩이 풀리면 다시 정면
}
