// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopScroll.h"

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
        ItemVerticalBox->AddChildToVerticalBox(NewSlot);
        ActiveSlots.Add(NewSlot);

        CurrentIndex = ActiveSlots.Num() - 1;
    }

    SelectInit();
}

void UShopScroll::MoveSelection(FIntPoint Direction)
{
    if (ActiveSlots.Num() == 0) return;

    const int32 NumPerRow = 5; // WrapBox 기준 가정
    const int32 MaxIndex = ActiveSlots.Num() - 1;

    int32 NextIndex = CurrentIndex;

    if (Direction.X != 0) // 좌우
    {
        NextIndex += Direction.X;
    }
    else if (Direction.Y != 0) // 상하
    {
        NextIndex += Direction.Y * NumPerRow;
    }

    NextIndex = FMath::Clamp(NextIndex, 0, MaxIndex);

    if (NextIndex != CurrentIndex)
    {
        ScrollBox->ScrollWidgetIntoView(ActiveSlots[NextIndex], true, EDescendantScrollDestination::IntoView);
        ActiveSlots[CurrentIndex]->SetSelected(false);
        ActiveSlots[NextIndex]->SetSelected(true);
        CurrentIndex = NextIndex;
    }

    OnHighlightChanged.Broadcast(CurrentIndex);
}

void UShopScroll::SelectInit()
{
    if (ActiveSlots.IsEmpty())
        return;

    for (UShopSlot* slot : ActiveSlots)
    {
        slot->SetSelected(false);
    }

    ActiveSlots[CurrentIndex]->SetSelected(true);

    OnHighlightChanged.Broadcast(CurrentIndex);
}

FItemData UShopScroll::GetItemDataAtIndex(int32 Index) const
{
    if (ActiveSlots.IsValidIndex(Index) && ActiveSlots[Index] != nullptr)
    {
        return ActiveSlots[Index]->GetItemData();
    }

    return FItemData();
}
