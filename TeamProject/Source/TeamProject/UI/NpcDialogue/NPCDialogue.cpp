// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NpcDialogue/NPCDialogue.h"

#include "SubSystem/UI/QuestDialogueManager.h"


void UNPCDialogue::OnCreated()
{
	Super::OnCreated();

    InitUI();

    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->OnDialogueUpdated.AddDynamic(this, &UNPCDialogue::RefreshDialogue);
    }

}

void UNPCDialogue::InitUI()
{

}

void UNPCDialogue::CloseDialogue()
{

}

void UNPCDialogue::RefreshDialogue(const FNPCDialogueTableRow& QuestData)
{
    TArray<FString> DialogueArr = QuestData.DialogueArr;

    for (FString Dialogue : DialogueArr)
    {
        MyMultiLineBox->SetText(FText::FromString(Dialogue));
    }
}
