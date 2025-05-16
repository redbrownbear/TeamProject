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
    // 1. �������� �����̴� �÷��̾��� �ݸ����� �������Ǹ� �÷��̾�� �ü� ����
    // 2. �ݸ��� �������� Ǯ���� �ٽ� ����
}
