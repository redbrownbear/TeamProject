// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickSlot/QuickItemSlot.h"
#include "SubSystem/PlayerManager.h"

void UQuickItemSlot::NativeConstruct()
{
	SetIsFocusable(true);
    LoadIconTexture();

    HighlightBg->SetVisibility(ESlateVisibility::Collapsed);
	EquipedBg->SetVisibility(ESlateVisibility::Collapsed);
    CategoryIconImg->SetVisibility(ESlateVisibility::Collapsed);
}

void UQuickItemSlot::LoadIconTexture()
{
	WeaponCategory = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, (TEXT("/Game/Resources/UI/UIIcon/Icon_SwordSelect.Icon_SwordSelect"))));
	ShieldCategory = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/UIIcon/Icon_ShieldSelect.Icon_ShieldSelect")));
	BowCategory = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/UIIcon/Icon_ArrowSelect.Icon_ArrowSelect")));
	ArrowCategory = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/UIIcon/Icon_ArrowSelect.Icon_ArrowSelect")));
}

void UQuickItemSlot::InitFirstSlot(eEquipParts Parts)
{
	UTexture2D* LoadedTexture = GetCategoryIcon(Parts);

	if (LoadedTexture)
	{
        CategoryIconImg->SetVisibility(ESlateVisibility::Visible);
        CategoryIconImg->SetBrushFromTexture(LoadedTexture);
	}

    Icon->SetVisibility(ESlateVisibility::Hidden);
	Value->SetVisibility(ESlateVisibility::Hidden);
	Amount->SetVisibility(ESlateVisibility::Hidden);
	HighlightBg->SetVisibility(ESlateVisibility::Hidden);
	EquipedBg->SetVisibility(ESlateVisibility::Hidden);

	SetVisibility(ESlateVisibility::Visible);
	
}

UTexture2D* UQuickItemSlot::GetCategoryIcon(eEquipParts Parts) const
{
    switch (Parts)
    {

	case eEquipParts::RIGHT:
		return WeaponCategory;
	
	case eEquipParts::LEFT:
		return ShieldCategory;
	
	case eEquipParts::BOWRIGHT:
		return BowCategory;
	
	case eEquipParts::ARROWLEFT:
		return ArrowCategory;   
    }

	return nullptr;
}

void UQuickItemSlot::SetItemData(const FItemData& Item)
{
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

void UQuickItemSlot::SetSelected(bool bIsSelected)
{
    HighlightBg->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UQuickItemSlot::SetEquiped(bool bIsEquiped)
{
    EquipedBg->SetVisibility(bIsEquiped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UQuickItemSlot::SetEquipDate()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    PlayerManager->SetEquipData(ItemData);
}
