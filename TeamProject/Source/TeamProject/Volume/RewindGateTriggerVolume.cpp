// Fill out your copyright notice in the Description page of Project Settings.


#include "Volume/RewindGateTriggerVolume.h"

#include "Actors/Object/RewindGate.h"
#include "Actors/Object/MetalActor.h"

#include "Components/MetalComponent/MetalComponent.h"

#include "SubSystem/Puzzle/EventManager.h"

ARewindGateTriggerVolume::ARewindGateTriggerVolume()
{
}

void ARewindGateTriggerVolume::BeginPlay()
{
    Super::BeginPlay();

    check(TargetGate != nullptr);

    OnActorBeginOverlap.AddDynamic(this, &ARewindGateTriggerVolume::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ARewindGateTriggerVolume::OnOverlapEnd);
}

void ARewindGateTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor)
    {
        if (OtherActor->GetComponentByClass<UMetalComponent>())
        {
            UE_LOG(LogTemp, Warning, TEXT("ARewindGateTriggerVolume::OnOverlapBegin // MetalCompnent checked"))
            TargetGate->CloseFinalGate();
        }

    }
}

void ARewindGateTriggerVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor)
    {
        if (OtherActor->GetComponentByClass<UMetalComponent>())
        {
            UE_LOG(LogTemp, Warning, TEXT("ARewindGateTriggerVolume::OnOverlapBegin // MetalCompnent checked"))
            TargetGate->OpenFinalGate();
        }
    }
}
