// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/QuestManager.h"

UQuestManager::UQuestManager()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> ShopTableObj(TEXT("/Game/Data/Quest/DT_Quest.DT_Quest"));

    if (ShopTableObj.Succeeded())
    {
        QuestDataTable = ShopTableObj.Object;
    }
    check(QuestDataTable);
}

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
    //데이터 확인을 위한 임시
    //퀘스트를 받아야 데이터를 저장하자
    LoadQuestData(QuestDataTable);
}

void UQuestManager::LoadQuestData(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
        return;
    }

    TArray<FName> RowNames = DataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FQuestDataRow* Row = DataTable->FindRow<FQuestDataRow>(RowName, "Populate QuestRow");
        if (Row)
        {
            if (TArray<FQuestDataRow>* FoundRowsPtr = QuestRowMap.Find(Row->QuestCharacter))
            {
                FoundRowsPtr->Add(*Row);
            }
            else // 존재하지 않으면 새 배열을 만들어 추가
            {
                TArray<FQuestDataRow> NewArray;
                NewArray.Add(*Row);
                QuestRowMap.Add(Row->QuestCharacter, NewArray);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}

TArray<FQuestDataRow> UQuestManager::GetQuestData(EQuestCharacter QuestChar) const
{
    const TArray<FQuestDataRow>* FoundRowsPtr = QuestRowMap.Find(QuestChar);
    if (!FoundRowsPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No shop data found for character: %s"), *UEnum::GetValueAsString(QuestChar));
        return {};
    }

    TArray<FQuestDataRow> ConstRows;
    for (const FQuestDataRow& Row : *FoundRowsPtr)
    {
        ConstRows.Add(Row); // 복사되지만 const 형태로 유지
    }

    return ConstRows;
}

void UQuestManager::ShowUI()
{
    TArray<FQuestDataRow> AllQuestList;

    for (const TPair<EQuestCharacter, TArray<FQuestDataRow>>& Pair : QuestRowMap)
    {
        AllQuestList.Append(Pair.Value);
    }
    
    UpdateQuest(AllQuestList); 
}

void UQuestManager::UpdateQuestData(EQuestCharacter QuestChar, const FQuestDataRow UpdateShopRow)
{
    if (TArray<FQuestDataRow>* ShopList = QuestRowMap.Find(QuestChar))
    {
        for (int32 i = 0; i < ShopList->Num(); ++i)
        {
            if ((*ShopList)[i].QuestCharacter == UpdateShopRow.QuestCharacter)
            {
                (*ShopList)[i] = UpdateShopRow;  // 값 복사
                break;
            }
        }

        //임시
        UpdateQuest(*ShopList);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No shop data found for character: %s"), *UEnum::GetValueAsString(QuestChar));
    }
}

void UQuestManager::UpdateQuest(const TArray<FQuestDataRow>& ShopList)
{
    OnQuestUpdated.Broadcast(ShopList);
}
