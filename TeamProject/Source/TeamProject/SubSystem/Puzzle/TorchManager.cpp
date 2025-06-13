// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/Puzzle/TorchManager.h"
#include "Actors/Object/TorchStand.h"

void UTorchManager::RegisterTorch(ATorchStand* Torch)
{
    if (Torch)
    {
        TorchList.Add(Torch);
    }
}

void UTorchManager::NotifyTorchLit(ATorchStand* Torch)
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