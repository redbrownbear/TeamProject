// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopDialogue.h"

#include "SubSystem/UI/UIManager.h"

void UShopDialogue::NativeConstruct()
{
    Super::NativeConstruct();

    ConfirmButton->OnClicked.AddDynamic(this, &UShopDialogue::OnConfirm);
    CancelButton->OnClicked.AddDynamic(this, &UShopDialogue::OnCancel);
}

void UShopDialogue::InitUI()
{
    ConfirmButton->SetVisibility(ESlateVisibility::Collapsed);
    CancelButton->SetVisibility(ESlateVisibility::Collapsed);
    ExtraButton->SetVisibility(ESlateVisibility::Collapsed);

    ActionLay->SetVisibility(ESlateVisibility::Visible);
}

void UShopDialogue::SetBuy()
{
    ConfirmButton->SetVisibility(ESlateVisibility::Visible);
    CancelButton->SetVisibility(ESlateVisibility::Visible);

    ActionLay->SetVisibility(ESlateVisibility::Hidden);

    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(TakeWidget());
}

void UShopDialogue::SetSell()
{
    ConfirmButton->SetVisibility(ESlateVisibility::Visible);
    CancelButton->SetVisibility(ESlateVisibility::Visible);

    ActionLay->SetVisibility(ESlateVisibility::Hidden);

    ConfrimText->SetText(FText::FromString(TEXT("판매")));
    CancelText->SetText(FText::FromString(TEXT("취소")));
}

void UShopDialogue::OnNavigate(const FInputActionValue& InputActionValue)
{

}

void UShopDialogue::OnConfirm()
{
    int a = 0;
    //구매 완료
}

void UShopDialogue::OnCancel()
{
    InitUI();

    //바인드 돌리기
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
