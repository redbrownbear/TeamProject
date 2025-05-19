// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/InventoryManager.h"

void UInventoryManager::AddItem(FItemData Itemdata)
{
	Items.Add(Itemdata);

	OnInventoryUpdated.Broadcast(Itemdata); // UI에게 알림
}
