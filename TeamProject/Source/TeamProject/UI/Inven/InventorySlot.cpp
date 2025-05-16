// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventorySlot.h"

void UInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventorySlot::InitSlot(const FItemData& Item)
{
    // 아이템 이미지, 텍스트 등 UI 바인딩
    //ItemIcon->SetI(Item.Icon);
    //ItemCount->SetText(FText::AsNumber(Item.Name));
    //SetVisibility(ESlateVisibility::Visible);
}
