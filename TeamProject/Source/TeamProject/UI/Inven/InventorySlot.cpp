// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventorySlot.h"

void UInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventorySlot::InitSlot(const FItemData& Item)
{
    // ������ �̹���, �ؽ�Ʈ �� UI ���ε�
    //ItemIcon->SetI(Item.Icon);
    //ItemCount->SetText(FText::AsNumber(Item.Name));
    //SetVisibility(ESlateVisibility::Visible);
}
