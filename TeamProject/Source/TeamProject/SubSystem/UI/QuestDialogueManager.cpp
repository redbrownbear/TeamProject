// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/QuestDialogueManager.h"



void UQuestDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UQuestDialogueManager::Initialize(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
        return;
    }

    static const FString ContextString(TEXT("Populate QuestRowMap"));

    TArray<FName> RowNames = DataTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        FNPCDialogueTableRow* Row = DataTable->FindRow<FNPCDialogueTableRow>(RowName, ContextString);

        if (Row)
        {
            if (Row->QuestCharacter != EQuestCharacter::None)
            {
                if (!QuestRowMap.Contains(Row->QuestCharacter))
                {
                    QuestRowMap.Add(Row->QuestCharacter, Row);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Duplicate QuestCharacter key found: %s"), *UEnum::GetValueAsString(Row->QuestCharacter));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}

const FNPCDialogueTableRow* UQuestDialogueManager::GetItemRow(EQuestCharacter Questchar) const
{
    auto FoundRow = QuestRowMap.Find(Questchar);
    if (!FoundRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("No row found for character: %s"), *UEnum::GetValueAsString(Questchar));
        return nullptr;
    }

    return *FoundRow;
}

void UQuestDialogueManager::ShowDialogue(EQuestCharacter Questchar)
{
    const FNPCDialogueTableRow* FoundRow = GetItemRow(Questchar);
    if (!FoundRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("No dialogue found for character: %s"), *UEnum::GetValueAsString(Questchar));
        return;
    }

    // 포인터 -> 참조로 변환해서 전달
    OnDialogueUpdated.Broadcast(*FoundRow);
}
