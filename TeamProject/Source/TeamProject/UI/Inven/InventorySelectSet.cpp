// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventorySelectSet.h"
#include "SubSystem/PlayerManager.h"
#include "Misc/TextUtil.h"

void UInventorySelectSet::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonUse->OnClicked.AddDynamic(this, &UInventorySelectSet::UseItme);
	ButtonCancel->OnClicked.AddDynamic(this, &UInventorySelectSet::Cancel);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UInventorySelectSet::BindDelegates()
{
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		OnItemEquipUpdated.AddDynamic(PlayerManager, &UPlayerManager::SetEquipData);
	}
}

void UInventorySelectSet::RemoveDelegate()
{
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		OnItemEquipUpdated.RemoveDynamic(PlayerManager, &UPlayerManager::SetEquipData);
	}
}

void UInventorySelectSet::ShowButton(bool bIsShow, FItemData InItemData)
{
	ItemData = InItemData;
	BindDelegates();

	SetVisibility(ESlateVisibility::Visible);

	switch (ItemData.eItemCategory)
	{
		case EItemCategory::IT_Weapon:
		case EItemCategory::IT_Arrow:
		case EItemCategory::IT_Shield:
		case EItemCategory::IT_Armor:
		{
			TextUse->SetText(FText::FromString(TextU(1005)));
			break;
		}
		case EItemCategory::IT_Food:
		case EItemCategory::IT_Material:
		{
			TextUse->SetText(FText::FromString(TextU(1004)));
			break;
		}
	}
}

void UInventorySelectSet::UseItme()
{
	switch (ItemData.eItemCategory)
	{
	case EItemCategory::IT_Weapon:
	case EItemCategory::IT_Arrow:
	case EItemCategory::IT_Shield:
	case EItemCategory::IT_Armor:
	{
		ChangeEquip();
	}
	break;
	case EItemCategory::IT_Food:
	case EItemCategory::IT_Material:
	{
		UseItem();
	}
	break;
	}
}

void UInventorySelectSet::Cancel()
{
	RemoveDelegate();
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySelectSet::ChangeEquip()
{
	OnItemEquipUpdated.Broadcast(ItemData);

	Cancel();
}

void UInventorySelectSet::UseItem()
{
	OnItemUseUpdated.Broadcast(ItemData);

	Cancel();
}