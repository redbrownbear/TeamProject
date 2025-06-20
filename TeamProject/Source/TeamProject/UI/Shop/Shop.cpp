// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Shop.h"
#include "Kismet/GameplayStatics.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/PlayerManager.h"

#include "GameFramework/PC_InGame.h"
#include "UI/HUD/MainHUD.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "UI/NpcDialogue/NPCDialogue.h"


void UShop::OnCreated()
{
    bIsBuyScroll = false;
    CurrentDisplayedCoin = 0;
    TargetCoin = 0;
    CoinStep = 1;
    UpdateInterval = 0.01f;

    SetShopOpen();
}

void UShop::ShowUI()
{
    Super::ShowUI();

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_UI);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
    }

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        CurrentDisplayedCoin = PlayerManager->GetRupee();
        CoinText->SetText(FText::FromString(FString::FromInt(CurrentDisplayedCoin)));
    }

    InitUI();
    BindDelegates();
}

void UShop::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->SetConversation(false);
    }

    RemoveDelegates();
    Super::HideUI(UShop::StaticClass());
}

void UShop::InitUI()
{
}

void UShop::SetRupeeUI()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        StartCoinEffect(PlayerManager->GetRupee());
    }
}

void UShop::StartCoinEffect(int32 FinalCoinValue)
{
    FString Text = CoinText->GetText().ToString();
    CurrentDisplayedCoin = FCString::Atoi(*Text); // ���� ǥ�õ� �� �������� ����

    TargetCoin = FinalCoinValue;

    int32 Difference = FMath::Abs(TargetCoin - CurrentDisplayedCoin);
    CoinStep = FMath::Clamp(Difference / 30, 1, 50); // ���̿� ���� ����

    GetWorld()->GetTimerManager().SetTimer(CoinUpdateTimerHandle, this, &UShop::UpdateCoinEffect, UpdateInterval, true);
}

void UShop::UpdateCoinEffect()
{
    if (CurrentDisplayedCoin == TargetCoin)
    {
        GetWorld()->GetTimerManager().ClearTimer(CoinUpdateTimerHandle);
        return;
    }

    if (CurrentDisplayedCoin < TargetCoin)
    {
        CurrentDisplayedCoin = FMath::Min(CurrentDisplayedCoin + CoinStep, TargetCoin);
    }
    else
    {
        CurrentDisplayedCoin = FMath::Max(CurrentDisplayedCoin - CoinStep, TargetCoin);
    }

    CoinText->SetText(FText::FromString(FString::FromInt(CurrentDisplayedCoin)));
}

void UShop::BindDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    if (QuestManager)
    {
        QuestManager->OnDialogueUpdated.AddDynamic(this, &UShop::RefreshDialogue);
    }

    UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
    if (ShopManager)
    {
        ShopManager->OnShopUpdated.AddDynamic(this, &UShop::RefreshShopList);
    }

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnInventoryAllUpdated.AddDynamic(this, &UShop::RefreshAllInventory);
    }

    BP_ShopScroll->OnShopHighlightChanged.AddDynamic(this, &UShop::RefreshDescription);

    BP_ShopSellScroll->OnShopDescriptionUpdated.AddDynamic(this, &UShop::RefreshDescriptionSellItem);
}

void UShop::RemoveDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->OnDialogueUpdated.RemoveDynamic(this, &UShop::RefreshDialogue);
    }

    UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
    if (ShopManager)
    {
        ShopManager->OnShopUpdated.RemoveDynamic(this, &UShop::RefreshShopList);
    }

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnInventoryAllUpdated.RemoveDynamic(this, &UShop::RefreshAllInventory);
    }

    BP_ShopScroll->OnShopHighlightChanged.RemoveDynamic(this, &UShop::RefreshDescription);

    BP_ShopSellScroll->OnShopDescriptionUpdated.RemoveDynamic(this, &UShop::RefreshDescriptionSellItem);
}

void UShop::SetShopOpen()
{
    BP_ShopDialogue->InitUI();

    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(TakeWidget());
}

void UShop::SetItemBuy()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    if (bIsBuyScroll == true)
        BP_ShopDialogue->SetBuy();

    if(bIsBuyScroll == false)
        BP_ShopDialogue->SetSell();

}

void UShop::RefreshAllInventory(const TArray<FItemData>& ItemDataList)
{
    SetShopOpen();

    BP_ShopScroll->SetVisibility(ESlateVisibility::Hidden);

    BP_ShopSellScroll->SetVisibility(ESlateVisibility::Visible);
    BP_ShopSellScroll->UpdateSlots(ItemDataList);
}

void UShop::OnNavigate(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    if (ActionValue.IsNearlyZero())
        return;

    if (BP_ShopDialogue->IsSelectItem())
        return;

    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        if (ActionValue.X > 0)
        {
            if(BP_ShopScroll->IsVisible())
                BP_ShopScroll->MoveSelection(FIntPoint(1, 0));
            
            if (BP_ShopSellScroll->IsVisible())
                BP_ShopSellScroll->MoveSelection(FIntPoint(1, 0));
        }
           
        else
        {
            if (BP_ShopScroll->IsVisible())
                BP_ShopScroll->MoveSelection(FIntPoint(-1, 0));
            if (BP_ShopSellScroll->IsVisible())
                BP_ShopSellScroll->MoveSelection(FIntPoint(-1, 0));
        }
           
    }
    else
    {
        if (ActionValue.Y > 0)
        {
            if (BP_ShopScroll->IsVisible())
                BP_ShopScroll->MoveSelection(FIntPoint(0, -1));
            if (BP_ShopSellScroll->IsVisible())
                BP_ShopSellScroll->MoveSelection(FIntPoint(0, -1));
        }      
        else
        {
            if (BP_ShopScroll->IsVisible())
                BP_ShopScroll->MoveSelection(FIntPoint(0, 1));
            if (BP_ShopSellScroll->IsVisible())
                BP_ShopSellScroll->MoveSelection(FIntPoint(0, 1));
        }
            
    }
}

void UShop::OnConfirm(const FInputActionValue& InputActionValue)
{
    if (BP_ShopDialogue->GetActionOverLay()->IsVisible() == false)
        return;

    SetItemBuy();
}

void UShop::OnCancel(const FInputActionValue& InputActionValue)
{
    if (BP_ShopDialogue->GetActionOverLay()->IsVisible() == false)
    {
        BP_ShopDialogue->OnCancel();
        return;
    }

    HideUI(UShop::StaticClass());

    UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    UConversationManagerComponent* ConversationManager = Cast<UConversationManagerComponent>(PC_InGame->Npc->GetComponentByClass(UConversationManagerComponent::StaticClass()));
    check(ConversationManager);

    PC_InGame->Npc->SetCurrentDialogueType(EDialogType::Shop);

    EQuestCharacter QuestCharacter = PC_InGame->Npc->GetData()->QuestCharacter;
    EDialogType DialogType = PC_InGame->Npc->GetCurrentDialogueType();

    DialogueID = ConversationManager->GetDialogueID(ConversationManager->GetNpcDialogueTable(), QuestCharacter, DialogType);

    if (UIManager && QuestManager)
    {
        if (QuestManager->IsConversation())
            return;

        UIManager->ShowUI(UNPCDialogue::StaticClass());
        QuestManager->ShowDialogue(QuestCharacter, DialogueID);
    }
}

void UShop::OnCategoryLeft(const FInputActionValue& InputActionValue)
{
    BP_ShopSellScroll->MoveCategory(true);
}

void UShop::OnCategoryRight(const FInputActionValue& InputActionValue)
{
    BP_ShopSellScroll->MoveCategory(false);
}

void UShop::SetDialogueData(EQuestCharacter InQuestChar, int32 InDialogueID)
{
    QuestChar = InQuestChar;
    DialogueID = InDialogueID;

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
}

void UShop::OnBuy()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        CurrentDisplayedCoin = PlayerManager->GetRupee();
    }

    BP_ShopScroll->ItemBuy();
}

void UShop::OnSell()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        CurrentDisplayedCoin = PlayerManager->GetRupee();
    }

	BP_ShopSellScroll->ItemSell();
}

void UShop::OnNextDialogue(const FInputActionValue& InputActionValue)
{
}

void UShop::RefreshDialogue(const FNPCDialogueTableRow& QuestData)
{
    BP_ShopDialogue->RefreshDialogue(QuestData);
}

void UShop::RefreshDescription(int32 CurrentIdx)
{
   const FItemData& ItemData = BP_ShopScroll->GetItemDataAtIndex(CurrentIdx);

    BP_ShopDescription->RefreshUI(ItemData);
}

void UShop::RefreshDescriptionSellItem(const FItemData& ItemData)
{
    BP_ShopDescription->RefreshUI(ItemData);
}

void UShop::RefreshShopList(const TArray<FShopDataRow>& ShopList)
{
    SetShopOpen();
    SetRupeeUI();

    if (bIsBuyScroll == false)
        return;

    BP_ShopSellScroll->SetVisibility(ESlateVisibility::Hidden);

    BP_ShopScroll->SetVisibility(ESlateVisibility::Visible);
    BP_ShopScroll->UpdateSlots(ShopList);
}
