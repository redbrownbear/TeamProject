// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/QuestDialogueManager.h"



void UQuestDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataTable is not assigned in %s"), *GetName());
        return;
    }

    ItemRowMap.Empty();

    const FString Context = TEXT("Loading Dialogue Table");

    TArray<FNPCDialogueTableRow*> AllRows;
    ItemDataTable->GetAllRows(Context, AllRows);

    TSet<EQuestCharacter> SeenCharacters;

    for (const FNPCDialogueTableRow* RowPtr : AllRows)
    {
        if (!RowPtr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Null row in DataTable"));
            continue;
        }

        const FNPCDialogueTableRow& Row = *RowPtr;

        EQuestCharacter Character = Row.QuestCharacter;

        if (Character == EQuestCharacter::None)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid character enum in row"));
            continue;
        }

        if (SeenCharacters.Contains(Character))
        {
            UE_LOG(LogTemp, Error, TEXT("Duplicate character entry: %s"), *UEnum::GetValueAsString(Character));
            continue;
        }

        //SeenCharacters.Add(Character);
        //ItemRowMap.Add(Character, Row);
    }

    UE_LOG(LogTemp, Log, TEXT("ItemRowMap loaded with %d entries"), ItemRowMap.Num());
}

const FNPCDialogueTableRow* UQuestDialogueManager::GetItemRow(EQuestCharacter Questchar) const
{
    auto FoundRow = ItemRowMap.Find(Questchar);
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
