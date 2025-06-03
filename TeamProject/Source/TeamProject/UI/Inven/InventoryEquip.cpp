// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventoryEquip.h"

void UInventoryEquip::NativeConstruct()
{
	Super::NativeConstruct();

	BoxItemOption->SetVisibility(ESlateVisibility::Hidden);
	BoxItemOptionCompare->SetVisibility(ESlateVisibility::Hidden);
	BoxItemHp->SetVisibility(ESlateVisibility::Hidden);

	TextItemName->SetText(FText::FromString(""));
	TextDescription->SetText(FText::FromString(""));
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

void UInventoryEquip::SetEquipMakeData(const TArray<FItemData>& ItemDataList)
{
	TMap<eEquipParts, FItemData> ItemDataMap;

	for (const FItemData& ItemData : ItemDataList)
	{	
		eEquipParts Parts = eEquipParts::NONE;

		switch (ItemData.eItemCategory)
		{
		case EItemCategory::IT_Weapon:
			Parts = eEquipParts::RIGHT;
			break;
		case EItemCategory::IT_Shield:
			Parts = eEquipParts::LEFT;
			break;
		case EItemCategory::IT_Arrow:
			Parts = ItemData.bIsArrow ? eEquipParts::ARROWLEFT : eEquipParts::BOWRIGHT;
			break;
		case EItemCategory::IT_Armor:
			switch (ItemData.eArmorKind)
			{
			case EArmorKind::HEAD:
				Parts = eEquipParts::HEAD;
				break;
			case EArmorKind::ARMOR:
				Parts = eEquipParts::ARMOR;
				break;
			case EArmorKind::LEG:
				Parts = eEquipParts::UNDER;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		if (Parts != eEquipParts::NONE)
		{
			ItemDataMap.Add(Parts, ItemData);
		}
	}

	RefreshEquip(ItemDataMap);
}

void UInventoryEquip::RefreshEquip(const TMap<eEquipParts, FItemData>& ItemDataMap)
{
	for (const TPair<eEquipParts, FItemData>& Pair : ItemDataMap)
	{
		UTexture2D* LoadedTexture = Pair.Value.ItemIcon.LoadSynchronous();
		if (LoadedTexture)
		{
			switch (Pair.Key)
			{
			case eEquipParts::HEAD:
				HeadImg->SetBrushFromTexture(LoadedTexture);
				break;
			case eEquipParts::ARMOR:
				ArmorImg->SetBrushFromTexture(LoadedTexture);
				break;
			case eEquipParts::UNDER:
				UnderImg->SetBrushFromTexture(LoadedTexture);
				break;
			case eEquipParts::RIGHT:
				RightImg->SetBrushFromTexture(LoadedTexture);
				break;
			case eEquipParts::LEFT:
				LeftImg->SetBrushFromTexture(LoadedTexture);
				break;
			case eEquipParts::BOWRIGHT:
				RightBowImg->SetBrushFromTexture(LoadedTexture);
				break;
			case eEquipParts::ARROWLEFT:
				LeftArrowImg->SetBrushFromTexture(LoadedTexture);
				break;
			}
		}
	}
}
