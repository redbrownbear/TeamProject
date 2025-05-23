// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/QuestDialogueManager.h"


UQuestDialogueManager::UQuestDialogueManager()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DialogueTableObj(TEXT("/Game/Data/NPC/DT_NPCDialogue.DT_NPCDialogue"));

    if (DialogueTableObj.Succeeded())
    {
        QuestDataTable = DialogueTableObj.Object;
    }
    check(QuestDataTable);
}

void UQuestDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
    check(QuestDataTable);

    LoadDialogueData(QuestDataTable);
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
            // 원본 Row 복사본 생성
            FNPCDialogueTableRow* NewRow = new FNPCDialogueTableRow(*Row);
            TSharedPtr<const FNPCDialogueTableRow> SharedRow = MakeShareable(NewRow);

            TArray<TSharedPtr<const FNPCDialogueTableRow>>* FoundRowsPtr = QuestRowMap.Find(Row->QuestCharacter);
            if (FoundRowsPtr)
            {
                FoundRowsPtr->Add(SharedRow);
            }
            else
            {
                QuestRowMap.Add(Row->QuestCharacter, TArray<TSharedPtr<const FNPCDialogueTableRow>>{ SharedRow });
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
        }
    }
}

TArray<TSharedPtr<const FNPCDialogueTableRow>> UQuestDialogueManager::GetDialogueData(EQuestCharacter QuestChar) const
{
    const TArray<TSharedPtr<const FNPCDialogueTableRow>>* FoundRowsPtr = QuestRowMap.Find(QuestChar);
    if (!FoundRowsPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No dialogue found for character: %s"), *UEnum::GetValueAsString(QuestChar));
        
        return  TArray<TSharedPtr<const FNPCDialogueTableRow>>{}; // 빈 배열 반환
    }

    return *FoundRowsPtr;
}

void UQuestDialogueManager::ShowDialogue(EQuestCharacter QuestChar, int32 DialogueID)
{
    TArray<TSharedPtr<const FNPCDialogueTableRow>> DialogueRows = GetDialogueData(QuestChar);
    TSharedPtr<const FNPCDialogueTableRow> FoundRow = nullptr;

    for (TSharedPtr<const FNPCDialogueTableRow> Row : DialogueRows)
    {
        if (Row->CurrentDialogueID == DialogueID)
        {
            FoundRow = Row;
            break;
        }
    }

    if (!FoundRow)
    {
        check(FoundRow);
        return;
    }

    SetConversation(true);
    OnDialogueUpdated.Broadcast(*FoundRow);
}

void UQuestDialogueManager::HandleNextDialogueRequested(EQuestCharacter QuestChar, int32 NextID)
{
    ShowDialogue(QuestChar, NextID);
}