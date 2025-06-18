// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/Puzzle/EventManager.h"
#include "Actors/Object/TorchStand.h"

void UEventManager::NotifyOverlapTempleBall()
{
	OnOverlapTempleBall.Broadcast();
}

void UEventManager::RegisterTorch(ATorchStand* Torch)
{
    if (Torch)
    {
        TorchList.Add(Torch);
    }
}

void UEventManager::NotifyTorchLit(ATorchStand* Torch)
{
    if (Torch)
    {
        LitTorchSet.Add(Torch);

        if (LitTorchSet.Num() == TorchList.Num())
        {
            UE_LOG(LogTemp, Log, TEXT("All torches lit! Broadcasting from Subsystem."));
            OnAllTorchesLit.Broadcast();
        }
    }
}

void UEventManager::AssasinBossDead()
{
    OnAssasinBossDead.Broadcast();
}

void UEventManager::WeightFull()
{
	OnWeightFull.Broadcast();
}
