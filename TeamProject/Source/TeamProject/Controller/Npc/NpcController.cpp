#include "NpcController.h"
#include "Character/Npc/Npc.h"
#include "Components/Npc/NpcFSMComponent.h"
#include "Kismet/KismetMathLibrary.h"

ANpcController::ANpcController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANpcController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 현재 목표 위치와 가까우면 다음 포인트로
    /*if (NpcFSMComponent && NpcFSMComponent->GetNpcState() == ENpcState::Stroll)
    {
        if (!NpcFSMComponent->StrollPathActor) return;

        FVector Target = NpcFSMComponent->StrollPathActor->GetSplinePointLocation(NpcFSMComponent->CurrentStrollIndex);
        FVector Current = GetPawn()->GetActorLocation();

        if (FVector::Dist(Current, Target) <= NpcFSMComponent->AcceptanceRadius)
        {
            NpcFSMComponent->CurrentStrollIndex = (NpcFSMComponent->CurrentStrollIndex + 1) % NpcFSMComponent->StrollPathActor->GetSplineMaxIndex();
            MoveToStrollPoint();
        }
    }*/
}

void ANpcController::BeginPlay()
{
	Super::BeginPlay();

    if (GetPawn())
    {
        NpcFSMComponent = GetPawn()->FindComponentByClass<UNpcFSMComponent>();
    }

    if (NpcFSMComponent)
    {
        NpcFSMComponent->SetOwner(Cast<ANpc>(GetPawn()));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NpcFSMComponent is NULL in NpcController!"));
    }
}

void ANpcController::Stroll()
{
    if (!NpcFSMComponent->StrollPathActor) { return; }

    MoveToStrollPoint();
}

void ANpcController::MoveToStrollPoint()
{
    if (!NpcFSMComponent->StrollPathActor) return;

    FVector Dest = NpcFSMComponent->StrollPathActor->GetSplinePointLocation(NpcFSMComponent->CurrentStrollIndex);
    MoveToLocation(Dest, NpcFSMComponent->AcceptanceRadius);

    //// @TODO 움직이다가 플레이어와 콜리전이 오버랩 될 시 Perceive UI 생성

    //if (bPerceive)
    //{
    //    // @TODO Create Perceive UI
    //}
}

void ANpcController::Interact()
{
    // @TODO Create Talk UI 
}

void ANpcController::Talk()
{
	// 대화 UI
}
