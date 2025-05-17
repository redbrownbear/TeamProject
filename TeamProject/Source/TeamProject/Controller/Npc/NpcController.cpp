#include "NpcController.h"
#include "Character/Npc/Npc.h"
#include "Components/FSM/Npc/NpcFSMComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

    if (GetPawn())
    {
        NpcFSMComponent = GetPawn()->FindComponentByClass<UNpcFSMComponent>();

        if (NpcFSMComponent)
        {
            NpcFSMComponent->SetOwner(Cast<ANpc>(GetPawn()));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NpcFSMComponent is NULL in NpcController!"));
    }
}
