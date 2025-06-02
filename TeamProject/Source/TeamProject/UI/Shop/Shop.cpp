// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Shop.h"
#include "Kismet/GameplayStatics.h"

#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/PlayerManager.h"

#include "GameFramework/PC_InGame.h"
#include "UI/HUD/MainHUD.h"


void UShop::OnCreated()
{
    InitUI();
    SetRupeeUI();

    SetShopOpen();
}

void UShop::ShowUI()
{
    Super::ShowUI();

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_Shop);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
    }

    BindDelegates();
}

void UShop::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);
    }

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
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->BindShopInput();
    }
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
    }

    BP_ShopScroll->OnShopHighlightChanged.AddDynamic(this, &UShop::RefreshDescription);
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

    BP_ShopScroll->OnShopHighlightChanged.RemoveDynamic(this, &UShop::RefreshDescription);
}

void UShop::SetShopOpen()
{

    BP_ShopDialogue->InitUI();
}

void UShop::SetItemBuy()
{
    BP_ShopDialogue->SetBuy();
}

void UShop::OnNavigate(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    // Deadzone 방지
    if (ActionValue.IsNearlyZero())
        return;

    // 가장 강한 방향 하나만 해석
    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        // 좌우
        if (ActionValue.X > 0)
            BP_ShopScroll->MoveSelection(FIntPoint(1, 0));
        else
            BP_ShopScroll->MoveSelection(FIntPoint(-1, 0));
    }
    else
    {
        // 상하
        if (ActionValue.Y > 0)
            BP_ShopScroll->MoveSelection(FIntPoint(0, -1));
        else
            BP_ShopScroll->MoveSelection(FIntPoint(0, 1));
    }
}

void UShop::OnConfirm()
{
    SetItemBuy();
}

void UShop::OnCancel()
{
    HideUI(UShop::StaticClass());
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

void UShop::RefreshShopList(const TArray<FShopDataRow>& ShopList)
{
    BP_ShopScroll->UpdateSlots(ShopList);
}
