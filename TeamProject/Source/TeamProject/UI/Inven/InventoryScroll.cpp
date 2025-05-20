// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventoryScroll.h"

#include "SubSystem/UI/InventoryManager.h"

void UInventoryScroll::NativeConstruct()
{
    Super::NativeConstruct();

    InitializePool(100);//미리 100개의 아이콘을 만들어 놓는다.
    check(ItemWrapBox);
}

void UInventoryScroll::InitializePool(int32 PreloadCount)
{
    for (int32 i = 0; i < PreloadCount; ++i)
    {
        UInventorySlot* PoolSlot = CreateWidget<UInventorySlot>(GetWorld(), SlotWidgetClass);
        PoolSlot->RemoveFromParent();
        PooledSlots.Add(PoolSlot);
    }
}

void UInventoryScroll::AddItemSlot(const FItemData& NewItem)
{
    if (!SlotWidgetClass) 
        return;

    UInventorySlot* NewSlot = nullptr;

    if (PooledSlots.Num() > 0)
    {
        NewSlot = PooledSlots.Pop();
    }
    else
    {
        NewSlot = CreateWidget<UInventorySlot>(this, SlotWidgetClass);
    }

    if (!NewSlot) 
        return;

    NewSlot->SetItemData(NewItem);
    ItemWrapBox->AddChildToWrapBox(NewSlot);
    ActiveSlots.Add(NewSlot);

    SelectInit();
}

void UInventoryScroll::UpdateSlots(const TArray<FItemData>& NewItemList)
{
    for (UInventorySlot* ActiveSlot : ActiveSlots)
    {
        ActiveSlot->RemoveFromParent();
        PooledSlots.Add(ActiveSlot);
    }
    ActiveSlots.Empty();

    for (const FItemData& Item : NewItemList)
    {
        UInventorySlot* NewSlot = nullptr;

        if (PooledSlots.Num() > 0)
        {
            NewSlot = PooledSlots.Pop();
        }
        else
        {
            NewSlot = CreateWidget<UInventorySlot>(GetWorld(), SlotWidgetClass);
        }

        NewSlot->SetItemData(Item);
        ItemWrapBox->AddChildToWrapBox(NewSlot);
        ActiveSlots.Add(NewSlot);

        CurrentIndex = ActiveSlots.Num() - 1;
    }

    SelectInit();
}

void UInventoryScroll::MoveSelection(FIntPoint Direction)
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
        ActiveSlots[CurrentIndex]->SetSelected(false);
        ActiveSlots[NextIndex]->SetSelected(true);
        CurrentIndex = NextIndex;
    }
}

void UInventoryScroll::SelectInit()
{
    if (ActiveSlots.IsEmpty())
        return;

    for (UInventorySlot* slot : ActiveSlots)
    {
        slot->SetSelected(false);
    }

    ActiveSlots[CurrentIndex]->SetSelected(true);
}
