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

            if (TArray<FShopDataRow>* FoundRowsPtr = CurrentShopRowMap.Find(Row->QuestCharacter))
            {
                FoundRowsPtr->Add(*Row);
            }
            else 
            {
                TArray<FShopDataRow> NewArray;
                NewArray.Add(*Row);
                CurrentShopRowMap.Add(Row->QuestCharacter, NewArray);
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
    const TArray<FShopDataRow>* FoundRowsPtr = CurrentShopRowMap.Find(QuestChar);
    if (!FoundRowsPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No shop data found for character: %s"), *UEnum::GetValueAsString(QuestChar));
        return {};
    }

    TArray<FShopDataRow> ConstRows;
    for (const FShopDataRow& Row : *FoundRowsPtr)
    {
        ConstRows.Add(Row); 
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
    if (TArray<FShopDataRow>* ShopList = CurrentShopRowMap.Find(QuestChar))
    {
        for (int32 i = 0; i < ShopList->Num(); ++i)
        {
            if ((*ShopList)[i].ItemData.UniqueID == UpdateShopRow.ItemData.UniqueID)
            {
                (*ShopList)[i] = UpdateShopRow;  
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


bool UShopManager::CheckSoldout()
{
    // ItemCount == 0 이면 매진 처리
    if (SelectedShopItem.ItemData.ItemCount != 0) return false;

    return true;
}

bool UShopManager::CanIBuyIt()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    int32 Rupee = PlayerManager->GetRupee();

    if (Rupee < SelectedShopItem.ItemData.price)
    {
        return false;
    }

    return true;
}

void UShopManager::AddItemInventory()
{
    if (SelectedShopItem.ItemData.ItemCount != 0)
    {
        UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
        if (PlayerManager)
        {
            PlayerManager->SetInvenData(SelectedShopItem.ItemData);
        }
    }
}

void UShopManager::SubtractItemInventory()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->RemoveItemByUniqueID(SelectedShopItem.ItemData.UniqueID);
    }
}

void UShopManager::AddPlayerRupee()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        int32 Rupee = PlayerManager->GetRupee();
        Rupee += SelectedShopItem.ItemData.price;

        PlayerManager->SetRupee(Rupee);
        OnRupeeChanged.Broadcast();
    }
}

void UShopManager::SubtractPlayerRupee()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        int32 Rupee = PlayerManager->GetRupee();
        Rupee -= SelectedShopItem.ItemData.price;

        PlayerManager->SetRupee(Rupee);
        OnRupeeChanged.Broadcast();
    }
}

void UShopManager::SetSelectedItem(const FItemData& InShopData)
{
    SelectedShopItem.ItemData = InShopData;
}

