// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/InventoryManager.h"

UInventoryManager::UInventoryManager()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> ShopTableObj(TEXT("/Game/Data/ItemData/DT_Item.DT_Item"));

    if (ShopTableObj.Succeeded())
    {
        ItemDataTable = ShopTableObj.Object;
    }
    check(ItemDataTable);
}

void UInventoryManager::Initialize(FSubsystemCollectionBase& Collection)
{
    LoadItemData(ItemDataTable);
}

void UInventoryManager::LoadItemData(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
        return;
    }

    TArray<FName> RowNames = DataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FItemData* Row = DataTable->FindRow<FItemData>(RowName, "Populate ItemRow");
        if (Row)
        {
            Items.Add(*Row);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}
