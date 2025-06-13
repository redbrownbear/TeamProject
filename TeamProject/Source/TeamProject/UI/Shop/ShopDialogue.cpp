// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopDialogue.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/PlayerManager.h"

#include "Components/ConversationComponent/ConversationManagerComponent.h"

#include "Actors/Npc/Npc.h"
#include "GameFramework/PC_InGame.h"

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

    bIsSelect = false;
}

void UShopDialogue::SetBuy()
{
    ConfirmButton->SetVisibility(ESlateVisibility::Visible);
    CancelButton->SetVisibility(ESlateVisibility::Visible);

    ActionLay->SetVisibility(ESlateVisibility::Hidden);

    ConfrimText->SetText(FText::FromString(TEXT("구매")));
    CancelText->SetText(FText::FromString(TEXT("취소")));

    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(TakeWidget());

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));  

    bIsSelect = true;
}

void UShopDialogue::SetSell()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    if (PlayerManager->GetAllItemData().Num() > 0)
    {
        ConfirmButton->SetVisibility(ESlateVisibility::Visible);
        CancelButton->SetVisibility(ESlateVisibility::Visible);

        ConfrimText->SetText(FText::FromString(TEXT("판매")));
        CancelText->SetText(FText::FromString(TEXT("취소")));

        bIsSelect = true;
        ActionLay->SetVisibility(ESlateVisibility::Hidden);

        APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
        PC_InGame->Npc->SetCurrentDialogueType(EDialogType::Sell);
    }
}

void UShopDialogue::OnNavigate(const FInputActionValue& InputActionValue)
{

}

void UShopDialogue::OnConfirm()
{ 
    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);

    UConversationManagerComponent* ConverSationManager = Cast<UConversationManagerComponent>(PC_InGame->Npc->GetComponentByClass(UConversationManagerComponent::StaticClass()));
    check(ConverSationManager);

    UShop* ShopUI = UIManager->FindUI<UShop>();
    if (ShopUI)
    {
        if (ShopUI->IsBuyScroll() == true)
            ShopUI->OnBuy();

        else
            ShopUI->OnSell();
    }

    InitUI();

    EQuestCharacter QuestChar = PC_InGame->Npc->GetData()->QuestCharacter;
    EDialogType DialogType = PC_InGame->Npc->GetCurrentDialogueType();

    int32 DialogueID = ConverSationManager->GetDialogueID(ConverSationManager->GetDataTable(), QuestChar, DialogType);
    QuestManager->ShowDialogue(PC_InGame->Npc->GetData()->QuestCharacter, DialogueID);
}

void UShopDialogue::OnCancel()
{
    InitUI();

    UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));

    UConversationManagerComponent* ConverSationManager = Cast<UConversationManagerComponent>(PC_InGame->Npc->GetComponentByClass(UConversationManagerComponent::StaticClass()));
    check(ConverSationManager);

    EQuestCharacter QuestChar = PC_InGame->Npc->GetData()->QuestCharacter;
    EDialogType DialogType = PC_InGame->Npc->GetCurrentDialogueType();
    
    int32 DialogueID = ConverSationManager->GetDialogueID(ConverSationManager->GetDataTable(), QuestChar, DialogType);
    QuestManager->ShowDialogue(PC_InGame->Npc->GetData()->QuestCharacter, DialogueID);
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
