// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopSlot.h"

void UShopSlot::NativeConstruct()
{
	HighlightBg->SetVisibility(ESlateVisibility::Collapsed);
}

void UShopSlot::SetItemData(const FItemData& Item)
{
    //������ ó�� �� UI ó��
    ItemData = Item;

    // ������ ��� ó���� ���ΰ�?
    UTexture2D* LoadedTexture = Item.ItemIcon.LoadSynchronous();
    if (LoadedTexture)
    {
        ItemImg->SetBrushFromTexture(LoadedTexture);
    }

    ItemText->SetText(FText::FromString(Item.Name));
    //ItemCount->SetText((FText::FromString("1")));
    ItemCount->SetText(FText::FromString(FString::FromInt(Item.ItemCount)));
    ItemPrice->SetText(FText::FromString(FString::FromInt(Item.price)));
    SetVisibility(ESlateVisibility::Visible);
}

void UShopSlot::SetSelected(bool bIsSelected)
{
	HighlightBg->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	NoneBg->SetVisibility(bIsSelected ? ESlateVisibility::Hidden: ESlateVisibility::Visible);
}
