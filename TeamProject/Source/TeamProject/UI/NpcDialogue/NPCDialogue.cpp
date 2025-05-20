// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NpcDialogue/NPCDialogue.h"

#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/UIManager.h"

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

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind = PC->InputComponent->BindAction("CloseInven", IE_Pressed, this, &UNPCDialogue::CloseUI);
    }
}

void UNPCDialogue::CloseUI()
{
    Super::CloseUI();
}

void UNPCDialogue::InitUI()
{
    ExtraButton->SetVisibility(ESlateVisibility::Hidden);
}

void UNPCDialogue::RefreshDialogue(const FNPCDialogueTableRow& QuestData)
{
    TArray<FString> DialogueArr = QuestData.DialogueArr;

    for (FString Dialogue : DialogueArr)
    {
        TextBox->SetText(FText::FromString(Dialogue));
    }
}
