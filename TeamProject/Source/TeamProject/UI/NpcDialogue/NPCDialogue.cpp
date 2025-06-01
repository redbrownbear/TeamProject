// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NpcDialogue/NPCDialogue.h"

#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/UI/UIManager.h"

#include "GameFramework/PC_InGame.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

void UNPCDialogue::OnCreated()
{
    InitUI();
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
        PC_InGame->BindDialogueInput();
    }

    BindDelegates();
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

    RemoveDelegates();

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

void UNPCDialogue::RemoveDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->OnDialogueUpdated.RemoveDynamic(this, &UNPCDialogue::RefreshDialogue);
    }
}

void UNPCDialogue::OnNavigate(const FInputActionValue& InputActionValue)
{
}

void UNPCDialogue::OnConfirm()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    PC_InGame->Npc->SetIsConfirmed(true);

    HideUI(UNPCDialogue::StaticClass());

    if (CurQuestChar == EQuestCharacter::Furiko)
    {
        bool IsQuest = PC_InGame->Npc->GetDoQuest();

        if (DialogueDataRow.bIsEndConversation && !IsQuest)
        {
            PC_InGame->Npc->SetDoQuest(true);
        }
        else
        {
            PC_InGame->Npc->SetDoQuest(false);
        }
    }   

    if (PC_InGame->Npc->GetData()->DialogType == EDialogType::Shop)
    {	
        UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
        check(UIManager);

        UShopManager* ShopManager = GetWorld()->GetGameInstance()->GetSubsystem<UShopManager>();
        check(ShopManager);

        UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
        check(QuestManager);

        if (UIManager && ShopManager && QuestManager)
        {
            UIManager->ShowUI(UShop::StaticClass());
            ShopManager->ShowUI(PC_InGame->Npc->GetData()->QuestCharacter);
            //QuestManager->ShowDialogue(PC_InGame->Npc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Store));
            QuestManager->ShowDialogue(PC_InGame->Npc->GetData()->QuestCharacter, QUESTCHARDIALOGUE_STORE);

            bool IsShopping = PC_InGame->Npc->GetShopping();
            bool IsBuying = PC_InGame->Npc->GetBuy();

            if (DialogueDataRow.bIsEndConversation && !IsShopping)
            {
                PC_InGame->Npc->SetShopping(true);
                // Create 상품 리스트 UI: 이후 항목 클릭했을 때 산다/만다 대화 나오게
                // 결정에 따라 SetBuy()에 인자 넣어주기
            }
            /*else if (DialogueDataRow.bIsEndConversation && IsShopping && IsBuying)
            {
                // 구매 했을 경우
                PC_InGame->Npc->SetBuy(true);
                PC_InGame->Npc->SetShopping(false);
            }
            else if (DialogueDataRow.bIsEndConversation && IsShopping && !IsBuying)
            {
                // 구매 안 할 경우
                PC_InGame->Npc->SetBuy(false);
                PC_InGame->Npc->SetShopping(false);
            }*/
            else
            {
                PC_InGame->Npc->SetShopping(false);
            }
        }
    }
}

void UNPCDialogue::OnCancel()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    PC_InGame->Npc->SetIsConfirmed(false);

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
        // Ÿ���� ���̸� ��� ��ü �ؽ�Ʈ ���
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