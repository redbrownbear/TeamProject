// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/Puzzle/EventManager.h"
#include "Actors/Object/TorchStand.h"

void UEventManager::NotifyOverlapTempleBall()
{
	OnOverlapTempleBall.Broadcast();
}

void UEventManager::AssasinBossDead()
{
    OnAssasinBossDead.Broadcast();
}
