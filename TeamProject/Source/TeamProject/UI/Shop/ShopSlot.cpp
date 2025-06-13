// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopSlot.h"

void UShopSlot::NativeConstruct()
{
	HighlightBg->SetVisibility(ESlateVisibility::Collapsed);

    if (HighlightBg)
        HighlightBg->SetColorAndOpacity(FLinearColor::White);

    if (NoneBg)
        NoneBg->SetColorAndOpacity(FLinearColor::White);
}

void UShopSlot::SetItemData(const FItemData& Item)
{
    ItemData = Item;

    UTexture2D* LoadedTexture = Item.ItemIcon.LoadSynchronous();
    if (LoadedTexture)
    {
        ItemImg->SetBrushFromTexture(LoadedTexture);
    }

    ItemText->SetText(FText::FromString(Item.Name));
    ItemCount->SetText(FText::FromString(FString::FromInt(Item.ItemCount)));
    ItemPrice->SetText(FText::FromString(FString::FromInt(Item.price)));
    SetVisibility(ESlateVisibility::Visible);
}

void UShopSlot::SetSelected(bool bIsSelected)
{
	HighlightBg->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	NoneBg->SetVisibility(bIsSelected ? ESlateVisibility::Hidden: ESlateVisibility::Visible);
}

void UShopSlot::SetSoldOut()
{
    if (HighlightBg)
        HighlightBg->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));

    if (NoneBg)
        NoneBg->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
}
