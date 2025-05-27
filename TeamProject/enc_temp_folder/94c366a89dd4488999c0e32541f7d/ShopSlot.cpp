// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopSlot.h"

void UShopSlot::NativeConstruct()
{
	HighlightBg->SetVisibility(ESlateVisibility::Collapsed);
}

void UShopSlot::SetItemData(const FItemData& Item)
{
    // 갯수를 어떻게 처리할 것인가?
    UTexture2D* LoadedTexture = Item.ItemIcon.LoadSynchronous();
    if (LoadedTexture)
    {
        ItemImg->SetBrushFromTexture(LoadedTexture);
    }

    ItemText->SetText(FText::FromString(Item.Name));
    ItemCount->SetText((FText::FromString("1")));
    ItemPrice->SetText(FText::FromString(FString::FromInt(Item.price)));
    SetVisibility(ESlateVisibility::Visible);
}

void UShopSlot::SetSelected(bool bIsSelected)
{
	HighlightBg->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	NoneBg->SetVisibility(bIsSelected ? ESlateVisibility::Hidden: ESlateVisibility::Visible);
}
