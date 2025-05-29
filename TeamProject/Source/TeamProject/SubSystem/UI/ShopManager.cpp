// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/ShopManager.h"

UShopManager::UShopManager()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> ShopTableObj(TEXT("/Game/Data/ItemData/DT_Shop.DT_Shop"));

    if (ShopTableObj.Succeeded())
    {
        ShopDataTable = ShopTableObj.Object;
    }
    check(ShopDataTable);
}

void UShopManager::Initialize(FSubsystemCollectionBase& Collection)
{
    check(ShopDataTable);

    //초기 데이터만
    LoadShopData(ShopDataTable);
}

void UShopManager::LoadShopData(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
        return;
    }

    TArray<FName> RowNames = DataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FShopDataRow* Row = DataTable->FindRow<FShopDataRow>(RowName, "Populate ShopRow");
        if (Row)
        {
            if (TArray<FShopDataRow>* FoundRowsPtr = ShopRowMap.Find(Row->QuestCharacter))
            {
                FoundRowsPtr->Add(*Row);
            }
            else // 존재하지 않으면 새 배열을 만들어 추가
            {
                TArray<FShopDataRow> NewArray;
                NewArray.Add(*Row);
                ShopRowMap.Add(Row->QuestCharacter, NewArray);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}

TArray<FShopDataRow> UShopManager::GetShopData(EQuestCharacter QuestChar) const
{
    const TArray<FShopDataRow>* FoundRowsPtr = ShopRowMap.Find(QuestChar);
    if (!FoundRowsPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No shop data found for character: %s"), *UEnum::GetValueAsString(QuestChar));
        return {};
    }

    TArray<FShopDataRow> ConstRows;
    for (const FShopDataRow& Row : *FoundRowsPtr)
    {
        ConstRows.Add(Row); // 복사되지만 const 형태로 유지
    }

    return ConstRows;
}

void UShopManager::UpdateShopData(EQuestCharacter QuestChar, const FShopDataRow UpdateShopRow)
{
    if (TArray<FShopDataRow>* ShopList = ShopRowMap.Find(QuestChar))
    {
        for (int32 i = 0; i < ShopList->Num(); ++i)
        {
            if ((*ShopList)[i].ItemData.ItemID == UpdateShopRow.ItemData.ItemID)
            {
                (*ShopList)[i] = UpdateShopRow;  // 값 복사
                break;
            }
        }

        UpdateItem(*ShopList);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No shop data found for character: %s"), *UEnum::GetValueAsString(QuestChar));
    }
}

void UShopManager::UpdateItem(const TArray<FShopDataRow>& ShopList)
{
    OnShopUpdated.Broadcast(ShopList);
}
