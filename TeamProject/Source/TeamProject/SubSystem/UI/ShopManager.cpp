// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/PlayerManager.h"

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

    //�ʱ� �����͸�
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
            Row->ItemData.UniqueID = FGuid::NewGuid().ToString();

            if (TArray<FShopDataRow>* FoundRowsPtr = ShopRowMap.Find(Row->QuestCharacter))
            {
                FoundRowsPtr->Add(*Row);
            }
            else // �������� ������ �� �迭�� ����� �߰�
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
        ConstRows.Add(Row); // ��������� const ���·� ����
    }

    return ConstRows;
}

void UShopManager::ShowUI(EQuestCharacter QuestChar, bool IsBuy)
{
    SetIsBuy(IsBuy);

    if (IsBuy)
    {
        TArray<FShopDataRow> ShopList = GetShopData(QuestChar);
        UpdateItem(ShopList);
    }
    else
    {
        UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
        if (PlayerManager)
        {
            PlayerManager->ShowInvenUI();
        }
    }
}

void UShopManager::UpdateShopData(EQuestCharacter QuestChar, const FShopDataRow UpdateShopRow)
{
    if (TArray<FShopDataRow>* ShopList = ShopRowMap.Find(QuestChar))
    {
        for (int32 i = 0; i < ShopList->Num(); ++i)
        {
            if ((*ShopList)[i].ItemData.UniqueID == UpdateShopRow.ItemData.UniqueID)
            {
                (*ShopList)[i] = UpdateShopRow;  // �� ����
                break;
            }
        }

        //�ӽ�
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
