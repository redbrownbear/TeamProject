// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventorySlot.h"

#include "SubSystem/UI/InventoryManager.h"

void UInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventorySlot::SetItemData(const FItemData& Item)
{
    // 아이템 이미지, 텍스트 등 UI 바인딩
    UTexture2D* LoadedTexture = Item.ItemIcon.LoadSynchronous();
    if (LoadedTexture)
    {
        Icon->SetBrushFromTexture(LoadedTexture);
    }

    Amount->SetText(FText::FromString(FString::SanitizeFloat(Item.Damage)));
    SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::SetSelected(bool bIsSelected)
{
    HighlightBg->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
