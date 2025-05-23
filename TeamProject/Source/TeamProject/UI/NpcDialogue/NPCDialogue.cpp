// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NpcDialogue/NPCDialogue.h"

#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/UIManager.h"

#include "GameFramework/PC_InGame.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

void UNPCDialogue::OnCreated()
{
    InitUI();
    BindDelegates();
}

void UNPCDialogue::ShowUI()
{
	Super::ShowUI();

    ConfirmButton->SetVisibility(ESlateVisibility::Hidden);
    CancelButton->SetVisibility(ESlateVisibility::Hidden);
    ExtraButton->SetVisibility(ESlateVisibility::Hidden);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {

        PC_InGame->ChangeInputContext(EInputContext::IC_Dialogue);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
        PC_InGame->BindDialogueInput(this);
    }
}

void UNPCDialogue::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);
    }
    if (PC_InGame->Npc)
    {
        ANpcController* Controller = Cast<ANpcController>(PC_InGame->Npc->GetController());
        if (Controller)
        {
            UConversationManagerComponent* TalkManager = Controller->GetConversationManager();
            if (TalkManager)
            {
                TalkManager->EndConversation();  
            }
        }
    }

    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->SetConversation(false);
    }

    Super::HideUI(UNPCDialogue::StaticClass());
}

void UNPCDialogue::InitUI()
{
    ConfirmButton->OnClicked.AddDynamic(this, &UNPCDialogue::OnConfirm);
    CancelButton->OnClicked.AddDynamic(this, &UNPCDialogue::OnCancel);
}

void UNPCDialogue::BindDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->OnDialogueUpdated.AddDynamic(this, &UNPCDialogue::RefreshDialogue);
    }
}

void UNPCDialogue::OnNavigate(const FInputActionValue& InputActionValue)
{
}

void UNPCDialogue::OnConfirm()
{
    HideUI(UNPCDialogue::StaticClass());
}

void UNPCDialogue::OnCancel()
{
    HideUI(UNPCDialogue::StaticClass());
}

void UNPCDialogue::OnNextDialogue(const FInputActionValue& InputActionValue)
{
    OnNextButtonClicked();
}

void UNPCDialogue::RefreshDialogue(const FNPCDialogueTableRow& QuestData)
{
    DialogueDataRow = QuestData;
    NextDialogueID = QuestData.NextDialogueID;
    CurQuestChar = QuestData.QuestCharacter;

    if (!DialogueDataRow.DialogueString.IsEmpty())
    {
        FullText = DialogueDataRow.DialogueString;;
        CurrentCharIndex = 0;
        bIsTyping = true;
        TextBox->SetText(FText::GetEmpty());
        GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &UNPCDialogue::UpdateTyping, 0.05f, true);
    }
}

void UNPCDialogue::UpdateTyping()
{
    if (CurrentCharIndex >= FullText.Len())
    {
        // 출력 완료
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        bIsTyping = false;
        return;
    }

    FString CurrentText = FullText.Left(CurrentCharIndex + 1);
    TextBox->SetText(FText::FromString(CurrentText));
    CurrentCharIndex++;
}

void UNPCDialogue::OnNextButtonClicked()
{
    if (bIsTyping)
    {
        // 타이핑 중이면 즉시 전체 텍스트 출력
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        TextBox->SetText(FText::FromString(FullText));
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
