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
    }

    BindDelegates();
}

void UNPCDialogue::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);
   
    if (PC_InGame->Npc)
    {
        ANpcController* Controller = Cast<ANpcController>(PC_InGame->Npc->GetController());
        if (Controller)
        {
            UConversationManagerComponent* TalkManager = PC_InGame->Npc->GetConversationManager();
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
    ExtraButton->OnClicked.AddDynamic(this, &UNPCDialogue::OnSell);
    CancelButton->OnClicked.AddDynamic(this, &UNPCDialogue::OnCancel);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->BindDialogueInput();
    }
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
            PC_InGame->Npc->SetCurrentDialogueType(EDialogType::Quest);
        }
        else
        {
            PC_InGame->Npc->SetDoQuest(false);
            PC_InGame->Npc->SetCurrentDialogueType(EDialogType::None);
        }
    }   

    EQuestCharacter QuestChar = PC_InGame->Npc->GetData()->QuestCharacter;
    EDialogType DialogType = PC_InGame->Npc->GetData()->DialogType;

    if (DialogType == EDialogType::Shop)
    {	
        UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
        check(UIManager);

        UShopManager* ShopManager = GetWorld()->GetGameInstance()->GetSubsystem<UShopManager>();
        check(ShopManager);

        UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
        check(QuestManager);

        UConversationManagerComponent* ConverSationManager = Cast<UConversationManagerComponent>(PC_InGame->Npc->GetComponentByClass(UConversationManagerComponent::StaticClass()));
        check(ConverSationManager);

        int32 DialogueID = ConverSationManager->GetDialogueID(ConverSationManager->GetDataTable(), QuestChar, DialogType);

        if (UIManager && ShopManager && QuestManager && ConverSationManager)
        {
            UIManager->ShowUI(UShop::StaticClass());
            //ShopManager->ShowUI(PC_InGame->Npc->GetData()->QuestCharacter, true);
            ShopManager->ShowUI(QuestChar, true);
            
            QuestManager->ShowDialogue(PC_InGame->Npc->GetData()->QuestCharacter, DialogueID);            

            bool IsShopping = PC_InGame->Npc->GetShopping();
            //bool IsBuying = PC_InGame->Npc->GetBuy();

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

void UNPCDialogue::OnSell()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));   
    check(PC_InGame);

    UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    UShopManager* ShopManager = GetWorld()->GetGameInstance()->GetSubsystem<UShopManager>();
    check(ShopManager);

    UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);

    UConversationManagerComponent* ConverSationManager = Cast<UConversationManagerComponent>(PC_InGame->Npc->GetComponentByClass(UConversationManagerComponent::StaticClass()));
    check(ConverSationManager);

    EQuestCharacter QuestChar = PC_InGame->Npc->GetData()->QuestCharacter;
    EDialogType DialogType = PC_InGame->Npc->GetCurrentDialogueType();

    if (DialogType == EDialogType::Shop)
    {
        PC_InGame->Npc->SetIsConfirmed(true);
        HideUI(UNPCDialogue::StaticClass());

        int32 DialogueID = ConverSationManager->GetDialogueID(ConverSationManager->GetDataTable(), QuestChar, DialogType);

        if (UIManager && ShopManager && QuestManager)
        {
            UIManager->ShowUI(UShop::StaticClass());
            ShopManager->ShowUI(PC_InGame->Npc->GetData()->QuestCharacter, false);

            QuestManager->ShowDialogue(PC_InGame->Npc->GetData()->QuestCharacter, DialogueID);
        }
    }
    else
    {
        PC_InGame->Npc->SetIsConfirmed(false);

        HideUI(UNPCDialogue::StaticClass());
    }
}

void UNPCDialogue::OnCancel()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    if (CurQuestChar == EQuestCharacter::Furiko)
    {
        switch (PC_InGame->Npc->GetCurrentDialogueType())
        {
        case EDialogType::Quest:
            PC_InGame->Npc->SetDoQuest(false);
            PC_InGame->Npc->SetCurrentDialogueType(EDialogType::None);
            break;
        default:
            break;
        }

        //if (DialogueDataRow.bIsEndConversation && IsQuest)
        //{
        //    PC_InGame->Npc->SetDoQuest(false);
        //    PC_InGame->Npc->SetCurrentDialogueType(EDialogType::None);
        //}
        //else
        //{
        //    PC_InGame->Npc->SetDoQuest(false);
        //    PC_InGame->Npc->SetCurrentDialogueType(EDialogType::None);
        //}
    }

    if (PC_InGame->Npc->GetCurrentDialogueType() == EDialogType::Shop)
    {
        PC_InGame->Npc->SetIsConfirmed(false);

    }

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
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);


    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        TextBox->SetText(FText::FromString(FullText));
        bIsTyping = false;
    }
    else
    {
        if (PC_InGame->Npc->GetCurrentDialogueType() == EDialogType::Shop)
        {
            ConfirmButton->SetVisibility(ESlateVisibility::Visible);
            CancelButton->SetVisibility(ESlateVisibility::Visible);
            ExtraButton->SetVisibility(ESlateVisibility::Visible);

            ConfrimText->SetText(FText::FromString(TEXT("구매")));
            CancelText->SetText(FText::FromString(TEXT("나가기")));
            ExtraText->SetText(FText::FromString(TEXT("판매")));

            CancelButton->SetRenderTranslation(FVector2D(0.0f, 0.0f));
        }

        if (DialogueDataRow.bIsEndConversation == true)
        {
            ConfirmButton->SetVisibility(ESlateVisibility::Visible);
            CancelButton->SetVisibility(ESlateVisibility::Visible);
            ExtraButton->SetVisibility(ESlateVisibility::Hidden);

            ConfrimText->SetText(FText::FromString(TEXT("확인")));
            CancelText->SetText(FText::FromString(TEXT("취소")));

            CancelButton->SetRenderTranslation(FVector2D(0.0f, -95.0f));

            return;
        }

        UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
        if (UIManager)
        {
            UIManager->OnDialogueNextRequested.Broadcast(CurQuestChar, NextDialogueID);
        }
    }
}