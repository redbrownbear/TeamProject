// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Shop.h"
#include "Kismet/GameplayStatics.h"

#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"

#include "GameFramework/PC_InGame.h"
#include "UI/HUD/MainHUD.h"


void UShop::OnCreated()
{
    SetCoinText(0);
}

void UShop::ShowUI()
{
    Super::ShowUI();

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_Inventory);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
        PC_InGame->BindShopInput(this);

        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(false);
    }

    BindDelegates();
}

void UShop::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);

        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(true);

    }

    RemoveDelegates();
    Super::HideUI(UInventory::StaticClass());

}

void UShop::SetCoinText(int32 CoinCount)
{
	if (CoinText)
	{
		CoinText->SetText(FText::FromString(FString::FromInt(CoinCount)));
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
}

void UShop::RemoveDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->OnDialogueUpdated.RemoveDynamic(this, &UShop::RefreshDialogue);
    }
}

void UShop::OnNavigate(const FInputActionValue& InputActionValue)
{
}

void UShop::OnConfirm()
{
}

void UShop::OnCancel()
{
}

void UShop::OnNextDialogue(const FInputActionValue& InputActionValue)
{
}

void UShop::RefreshDialogue(const FNPCDialogueTableRow& QuestData)
{
    BP_ShopDialogue->RefreshDialogue(QuestData);
}

void UShop::RefreshDescription(const FItemData& ItemData)
{
    BP_ShopDescription->RefreshUI(ItemData);
}

void UShop::RefreshShopList(const TArray<FShopDataRow>& ShopList)
{
    BP_ShopScroll->UpdateSlots(ShopList);
}
