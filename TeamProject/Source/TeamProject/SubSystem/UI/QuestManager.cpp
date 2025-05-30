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
            QuestArr.Add(*Row);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}

TArray<FQuestDataRow> UQuestManager::GetQuestData() const
{

    //TArray<FQuestDataRow> ConstRows;
    //for (const FQuestDataRow& Row : QuestArr)
    //{
    //    if (QuestChar == Row.QuestCharacter)
    //    {
    //        ConstRows.Add(Row); // 복사되지만 const 형태로 유지
    //    }
    //}

    return QuestArr;
}
