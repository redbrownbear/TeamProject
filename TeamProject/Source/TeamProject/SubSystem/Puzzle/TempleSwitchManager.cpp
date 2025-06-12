// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/Puzzle/TempleSwitchManager.h"

void UTempleSwitchManager::NotifyOverlapTempleBall()
{
	OnOverlapTempleBall.Broadcast();
}
