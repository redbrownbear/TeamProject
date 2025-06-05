// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventorySlot.h"

#include "SubSystem/UI/InventoryManager.h"

void UInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();

    SetIsFocusable(true);

    EquipedBg->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventorySlot::SetItemData(const FItemData& Item)
{
    // 아이템 이미지, 텍스트 등 UI 바인딩
    UTexture2D* LoadedTexture = Item.ItemIcon.LoadSynchronous();
    if (LoadedTexture)
    {
        Icon->SetBrushFromTexture(LoadedTexture);
    }

    Value->SetVisibility(ESlateVisibility::Visible);
    Amount->SetVisibility(ESlateVisibility::Visible);

    switch (Item.eItemCategory)
    {
    case EItemCategory::IT_Weapon:
        Value->SetText(FText::FromString(FString::FromInt(Item.Damage)));
        Amount->SetVisibility(ESlateVisibility::Hidden);
        break;
    case EItemCategory::IT_Arrow:  
        if (Item.bIsArrow)
        {
            Value->SetVisibility(ESlateVisibility::Hidden);
            Amount->SetText(FText::FromString(FString::Printf(TEXT("X %d"), Item.ItemCount)));
        }
        else
        {
            Amount->SetVisibility(ESlateVisibility::Hidden);
            Value->SetText(FText::FromString(FString::FromInt(Item.Damage)));
        }
        break;
    case EItemCategory::IT_Shield:
        Value->SetText(FText::FromString(FString::FromInt(Item.Damage)));
        Amount->SetVisibility(ESlateVisibility::Hidden);
        break;
    case EItemCategory::IT_Armor:
        Value->SetText(FText::FromString(FString::FromInt(Item.Damage)));
        Amount->SetVisibility(ESlateVisibility::Hidden);
        break;
    case EItemCategory::IT_Material:
        Value->SetVisibility(ESlateVisibility::Hidden);
        Amount->SetText(FText::FromString(FString::Printf(TEXT("X %d"), Item.ItemCount)));
        break;
    case EItemCategory::IT_Food:
        Value->SetVisibility(ESlateVisibility::Hidden);
        Amount->SetVisibility(ESlateVisibility::Hidden);
        break;
    }

    SetVisibility(ESlateVisibility::Visible);

    ItemData = Item;
}

void UInventorySlot::SetSelected(bool bIsSelected)
{
    HighlightBg->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInventorySlot::SetEquiped(bool bIsEquiped)
{
    EquipedBg->SetVisibility(bIsEquiped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}