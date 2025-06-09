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
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {
        QuestManager->SetConversation(false);
    }

    RemoveDelegates();
    Super::HideUI(UShop::StaticClass());

}

bool UShop::CheckSoldout()
{ 
    if (!BP_ShopDescription)
    {
        UE_LOG(LogTemp, Warning, TEXT("BP_ShopDescription is Null"));
    }

    FShopDataRow SelectedShopItem; 
    SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

    // ItemCount == 0 이면 매진 처리
    if (SelectedShopItem.ItemData.ItemCount != 0) return false;

    return true;
}

void UShop::AddItemInventory()
{    
    FShopDataRow SelectedShopItem;
    SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

    if (SelectedShopItem.ItemData.ItemCount != 0)
    {
        UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
        if (PlayerManager)
        {
            PlayerManager->SetInvenData(SelectedShopItem.ItemData);
        }
    }
}

void UShop::SubtractItemInventory()
{   
    FShopDataRow SelectedShopItem;
    SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->RemoveItemByUniqueID(SelectedShopItem.ItemData.UniqueID);
    }  
}

bool UShop::CanIBuyIt()
{
    FShopDataRow SelectedShopItem;
    SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    int32 Rupee = PlayerManager->GetRupee();

    if (Rupee < SelectedShopItem.ItemData.price)
    {
        return false;
    }

    return true;
}

void UShop::AddPlayerRupee()
{
    FShopDataRow SelectedShopItem;
    SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        int32 Rupee = PlayerManager->GetRupee();
        Rupee += SelectedShopItem.ItemData.price;

        PlayerManager->SetRupee(Rupee);
        SetRupeeUI();
    }
}

void UShop::SubtractPlayerRupee()
{
    FShopDataRow SelectedShopItem;
    SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        int32 Rupee = PlayerManager->GetRupee();
        Rupee -= SelectedShopItem.ItemData.price;

        PlayerManager->SetRupee(Rupee);
        SetRupeeUI();
    }
}

void UShop::AddShopItem()
{
    FShopDataRow SelectedShopItem;

    UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
    if (ShopManager)
    {
        SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();
    }

    TArray<UShopSlot*> ActiveSlots = BP_ShopScroll->GetActiveSlots();
    int32 Index = BP_ShopScroll->GetItemDataIndex();

    if (Index != INDEX_NONE)
    {
        UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
        if (PlayerManager)
        {
            SelectedShopItem.ItemData.ItemCount += 1;
            SelectedShopItem.InitialItemCount += 1;
            ShopManager->UpdateShopData(EQuestCharacter::Korok, SelectedShopItem);

            if (ActiveSlots.IsValidIndex(Index))
            {
                ActiveSlots[Index]->SetItemData(SelectedShopItem.ItemData);
            }
        }
    }
}

void UShop::SubtractShopItem()
{
    FShopDataRow SelectedShopItem;

    UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
    if (ShopManager)
    {
        SelectedShopItem.ItemData = BP_ShopDescription->GetCurrentItemData();

        TArray<UShopSlot*> ActiveSlots = BP_ShopScroll->GetActiveSlots();
        int32 Index = BP_ShopScroll->GetItemDataIndex();

        if (Index != INDEX_NONE)
        {
            if (SelectedShopItem.ItemData.ItemCount != 0)
            {
                UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
                if (PlayerManager)
                {
                    SelectedShopItem.ItemData.ItemCount -= 1;
                    SelectedShopItem.InitialItemCount -= 1;
                    ShopManager->UpdateShopData(EQuestCharacter::Korok, SelectedShopItem);

                    if (ActiveSlots.IsValidIndex(Index))
                    {
                        ActiveSlots[Index]->SetItemData(SelectedShopItem.ItemData);
                    }
                }
            }
        }
    }
}

void UShop::InitUI()
{
    //APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    //if (PC_InGame)
    //{
    //    PC_InGame->BindShopInput();
    //}
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

    // Deadzone ����
    if (ActionValue.IsNearlyZero())
        return;

    // ���� ���� ���� �ϳ��� �ؼ�
    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        // �¿�
        if (ActionValue.X > 0)
            BP_ShopScroll->MoveSelection(FIntPoint(1, 0));
        else
            BP_ShopScroll->MoveSelection(FIntPoint(-1, 0));
    }
    else
    {
        // ����
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
