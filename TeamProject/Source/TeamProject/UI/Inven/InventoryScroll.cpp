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
    }
}
