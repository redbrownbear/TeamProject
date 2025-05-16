// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventorySlot.h"

#include "SubSystem/UI/InventoryManager.h"

void UInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventorySlot::SetItemData(const FItemData& Item)
{
    // ������ �̹���, �ؽ�Ʈ �� UI ���ε�
    UTexture2D* LoadedTexture = Item.Icon.LoadSynchronous();
    if (LoadedTexture)
    {
        Icon->SetBrushFromTexture(LoadedTexture);
    }

    Amount->SetText(FText::FromString(Item.Name));
    SetVisibility(ESlateVisibility::Visible);
}
