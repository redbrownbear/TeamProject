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

    InitUI();
    SetRupeeUI();
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
        CoinText->SetText(FText::FromString(FString::FromInt(PlayerManager->GetRupee())));
    }
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
        ShopManager->OnRupeeChanged.AddDynamic(this, &UShop::SetRupeeUI);
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
        ShopManager->OnRupeeChanged.RemoveDynamic(this, &UShop::SetRupeeUI);
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
    UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
    check(ShopManager);

    if(ShopManager->IsBuy())
        BP_ShopDialogue->SetBuy();

    else
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
            BP_ShopScroll->MoveSelection(FIntPoint(1, 0));
            BP_ShopSellScroll->MoveSelection(FIntPoint(1, 0));
        }
           
        else
        {
            BP_ShopScroll->MoveSelection(FIntPoint(-1, 0));
            BP_ShopSellScroll->MoveSelection(FIntPoint(-1, 0));
        }
           
    }
    else
    {
        if (ActionValue.Y > 0)
        {
            BP_ShopScroll->MoveSelection(FIntPoint(0, -1));
            BP_ShopSellScroll->MoveSelection(FIntPoint(0, -1));
        }      
        else
        {
            BP_ShopScroll->MoveSelection(FIntPoint(0, 1));
            BP_ShopSellScroll->MoveSelection(FIntPoint(0, 1));
        }
            
    }
}

void UShop::OnConfirm(const FInputActionValue& InputActionValue)
{
    SetItemBuy();
}

void UShop::OnCancel(const FInputActionValue& InputActionValue)
{
    HideUI(UShop::StaticClass());

    UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    UConversationManagerComponent* ConversationManager = Cast<UConversationManagerComponent>(PC_InGame->Npc->GetComponentByClass(UConversationManagerComponent::StaticClass()));
    check(ConversationManager);

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

void UShop::SetDialogueData(EQuestCharacter InQuestChar, int32 InDialogueID)
{
    QuestChar = InQuestChar;
    DialogueID = InDialogueID;

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    PC_InGame->Npc->SetCurrentDialogueType(EDialogType::Shop);
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

    BP_ShopSellScroll->SetVisibility(ESlateVisibility::Hidden);

    BP_ShopScroll->SetVisibility(ESlateVisibility::Visible);
    BP_ShopScroll->UpdateSlots(ShopList);
}
