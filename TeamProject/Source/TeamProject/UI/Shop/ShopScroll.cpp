// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopScroll.h"
#include "GameFramework/PC_InGame.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Npc/Npc.h"

#include "SubSystem/UI/ShopManager.h"


void UShopScroll::NativeConstruct()
{
	Super::NativeConstruct();

	InitializePool(100);
}

void UShopScroll::InitializePool(int32 PreloadCount)
{
    for (int32 i = 0; i < PreloadCount; ++i)
    {
        UShopSlot* PoolSlot = CreateWidget<UShopSlot>(GetWorld(), SlotWidgetClass);
        PoolSlot->RemoveFromParent();
        PooledSlots.Add(PoolSlot);
    }
}

void UShopScroll::UpdateSlots(const TArray<FShopDataRow>& ShopList)
{
    for (UShopSlot* ActiveSlot : ActiveSlots)
    {
        ActiveSlot->RemoveFromParent();
        PooledSlots.Add(ActiveSlot);
    }
    ActiveSlots.Empty();

    for (const FShopDataRow& ShopItem : ShopList)
    {
        UShopSlot* NewSlot = nullptr;

        if (PooledSlots.Num() > 0)
        {
            NewSlot = PooledSlots.Pop();
        }
        else
        {
            NewSlot = CreateWidget<UShopSlot>(GetWorld(), SlotWidgetClass);
        }

        FItemData Item = ShopItem.ItemData;
        NewSlot->SetItemData(Item);

        if(Item.ItemCount <=0)
			NewSlot->SetSoldOut();

        ItemVerticalBox->AddChildToVerticalBox(NewSlot);
        ActiveSlots.Add(NewSlot);
    }

    SelectInit();
}

void UShopScroll::MoveSelection(FIntPoint Direction)
{
    if (ActiveSlots.Num() == 0) return;

    const int32 MaxIndex = ActiveSlots.Num() - 1;

    int32 NextIndex = CurrentIndex;

    if (Direction.Y != 0)
    {
        NextIndex += Direction.Y;
    }

    NextIndex = FMath::Clamp(NextIndex, 0, MaxIndex);

    if (NextIndex != CurrentIndex)
    {
        ScrollBox->ScrollWidgetIntoView(ActiveSlots[NextIndex], true, EDescendantScrollDestination::IntoView);
        ActiveSlots[CurrentIndex]->SetSelected(false);
        ActiveSlots[NextIndex]->SetSelected(true);
        CurrentIndex = NextIndex;
    }

    FItemData Itemdata = ActiveSlots[CurrentIndex]->GetItemData();

    if (UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>())
    {
        ShopManager->SetSelectedItem(Itemdata);
    }

    OnShopHighlightChanged.Broadcast(CurrentIndex);
}

void UShopScroll::SelectInit()
{
    if (ActiveSlots.IsEmpty())
        return;

    for (UShopSlot* slot : ActiveSlots)
    {
        slot->SetSelected(false);
    }

    CurrentIndex = 0;

    ActiveSlots[CurrentIndex]->SetSelected(true);

    FItemData Itemdata = ActiveSlots[CurrentIndex]->GetItemData();

    if (UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>())
    {
        ShopManager->SetSelectedItem(Itemdata);
    }

    OnShopHighlightChanged.Broadcast(CurrentIndex);
}

void UShopScroll::ItemBuy()
{
    FItemData CurItemData = GetItemDataAtIndex(CurrentIndex);

	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	check(PlayerManager);

	UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
	check(ShopManager);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    if (CurItemData.ItemCount <= 0)
    {
        PC_InGame->Npc->SetCurrentDialogueType(EDialogType::SoldOut);
        return;
    }

    if (CurItemData.price > PlayerManager->GetRupee())
    {
        PC_InGame->Npc->SetCurrentDialogueType(EDialogType::Cashless);
        return;
    }

	int32 Rupee = PlayerManager->GetRupee() - CurItemData.price;
    PlayerManager->SetRupee(Rupee);
	PlayerManager->SetInvenData(CurItemData);

    TArray<FShopDataRow> CurrentShopDataArray = ShopManager->GetShopData(PC_InGame->Npc->GetQuestCharacterType());
    int iShopDataIndex = -1;
    for (int32 i = 0; i < CurrentShopDataArray.Num(); ++i)
    {
        if (CurrentShopDataArray[i].ItemData.ItemCode == CurItemData.ItemCode)
        {
            CurrentShopDataArray[i].ItemData.ItemCount--;
            iShopDataIndex = i;
            break;
        }
    }
    ShopManager->UpdateShopData(PC_InGame->Npc->GetQuestCharacterType(), CurrentShopDataArray[iShopDataIndex]);

    PC_InGame->Npc->SetCurrentDialogueType(EDialogType::SuccessfulShopping);   
}

FItemData UShopScroll::GetItemDataAtIndex(int32 Index) const
{
    if (ActiveSlots.IsValidIndex(Index) && ActiveSlots[Index] != nullptr)
    {
        return ActiveSlots[Index]->GetItemData();
    }

    return FItemData();
}
