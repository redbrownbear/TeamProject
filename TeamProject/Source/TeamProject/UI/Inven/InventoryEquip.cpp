// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventoryEquip.h"

void UInventoryEquip::NativeConstruct()
{
	Super::NativeConstruct();

	BoxItemOption->SetVisibility(ESlateVisibility::Hidden);
	BoxItemOptionCompare->SetVisibility(ESlateVisibility::Hidden);
	BoxItemHp->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryEquip::RefreshDescription(const FItemData& ItemData)
{
	TextItemName->SetText(FText::FromString(ItemData.Name));
	TextDescription->SetText(FText::FromString(ItemData.Description));

	UTexture2D* LoadedTexture = ItemData.CategoryIcon.LoadSynchronous();
	if (LoadedTexture)
	{
		ItemSimpleIcon->SetBrushFromTexture(LoadedTexture);
	}

}
