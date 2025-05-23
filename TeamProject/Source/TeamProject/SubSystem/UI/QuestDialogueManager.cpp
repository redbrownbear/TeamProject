// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/QuestDialogueManager.h"



void UQuestDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
    UDataTable* LoadedTable = Cast<UDataTable>(
        StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/Data/NPC/DT_NPCDialogue.DT_NPCDialogue"))
    );

    LoadDialogueData(LoadedTable);
}

void UQuestDialogueManager::LoadDialogueData(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
        return;
    }

    TArray<FName> RowNames = DataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FNPCDialogueTableRow* Row = DataTable->FindRow<FNPCDialogueTableRow>(RowName, "Populate QuestRowMap");
        if (Row)
        {
            if (Row->QuestCharacter != EQuestCharacter::None)
            {
                TArray<const FNPCDialogueTableRow*>* FoundArray = QuestRowMap.Find(Row->QuestCharacter);
                if (FoundArray)
                {
                    FoundArray->Add(Row);
                }
                else
                {
                    QuestRowMap.Add(Row->QuestCharacter, TArray<const FNPCDialogueTableRow*>{ Row });
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}

TArray<const FNPCDialogueTableRow*> UQuestDialogueManager::GetDialogueData(EQuestCharacter QuestChar) const
{
    const TArray<const FNPCDialogueTableRow*>* FoundRows = QuestRowMap.Find(QuestChar);
    if (!FoundRows)
    {
        UE_LOG(LogTemp, Warning, TEXT("No dialogue found for character: %s"), *UEnum::GetValueAsString(QuestChar));
        
        return TArray<const FNPCDialogueTableRow*>{}; // ºó ¹è¿­ ¹ÝÈ¯
    }

    return *FoundRows;
}

void UQuestDialogueManager::ShowDialogue(EQuestCharacter QuestChar, int32 DialogueID)
{
    TArray<const FNPCDialogueTableRow*> DialogueRows = GetDialogueData(QuestChar);

    if (DialogueRows.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No dialogue found for character: %s"), *UEnum::GetValueAsString(QuestChar));
        return;
    }

    const FNPCDialogueTableRow* FoundRow = nullptr;
    for (const FNPCDialogueTableRow* Row : DialogueRows)
    {
        if (Row->CurrentDialogueID == DialogueID)
        {
            FoundRow = Row;
            break;
        }
    }

    if (!FoundRow) 
        return;

    SetConversation(true);

    OnDialogueUpdated.Broadcast(*FoundRow);
}

void UQuestDialogueManager::HandleNextDialogueRequested(EQuestCharacter QuestChar, int32 NextID)
{
    ShowDialogue(QuestChar, NextID);
}