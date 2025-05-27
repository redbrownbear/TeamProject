// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopDialogue.h"

#include "SubSystem/UI/UIManager.h"

void UShopDialogue::InitUI()
{
}

void UShopDialogue::SetData()
{
}

void UShopDialogue::RefreshDialogue(const FNPCDialogueTableRow& QuestData)
{
    DialogueDataRow = QuestData;
    NextDialogueID = QuestData.NextDialogueID;
    CurQuestChar = QuestData.QuestCharacter;

    if (!DialogueDataRow.DialogueString.IsEmpty())
    {
        FullText = DialogueDataRow.DialogueString;;
        CurrentCharIndex = 0;
        bIsTyping = true;
        DialogueText->SetText(FText::GetEmpty());
        GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &UShopDialogue::UpdateTyping, 0.05f, true);
    }
}

void UShopDialogue::UpdateTyping()
{
    if (CurrentCharIndex >= FullText.Len())
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        bIsTyping = false;
        return;
    }

    FString CurrentText = FullText.Left(CurrentCharIndex + 1);
    DialogueText->SetText(FText::FromString(CurrentText));
    CurrentCharIndex++;
}

void UShopDialogue::OnNextButtonClicked()
{
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        DialogueText->SetText(FText::FromString(FullText));
        bIsTyping = false;
    }
    else
    {
        if (DialogueDataRow.bIsEndConversation == true)
        {
            ConfirmButton->SetVisibility(ESlateVisibility::Visible);
            CancelButton->SetVisibility(ESlateVisibility::Visible);
            return;
        }

        UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
        if (UIManager)
        {
            UIManager->OnDialogueNextRequested.Broadcast(CurQuestChar, NextDialogueID);
        }
    }
}
