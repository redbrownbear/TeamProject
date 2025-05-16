// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventoryScroll.h"

void UInventoryScroll::NativeConstruct()
{
    Super::NativeConstruct();

    ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemScrollBox")));
    check(ScrollBox);

}

void UInventoryScroll::UpdateSlots(const TArray<FItemData>& NewItemList)
{
    int32 NumToUpdate = FMath::Min(NewItemList.Num(), SlotPool.Num());

    // 1. ���� ���� ���� + ������ �ʱ�ȭ
    for (int32 i = 0; i < NumToUpdate; ++i)
    {
        SlotPool[i]->SetVisibility(ESlateVisibility::Visible);
        SlotPool[i]->InitSlot(NewItemList[i]);
    }

    // 2. ���� ���� �� ���� ���� �� �߰�
    for (int32 i = NumToUpdate; i < NewItemList.Num(); ++i)
    {
        UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(GetWorld(), SlotWidgetClass);
        NewSlot->InitSlot(NewItemList[i]);
        SlotPool.Add(NewSlot);
        ScrollBox->AddChild(NewSlot);
    }

    // 3. ���� ���� �����
    for (int32 i = NewItemList.Num(); i < SlotPool.Num(); ++i)
    {
        SlotPool[i]->SetVisibility(ESlateVisibility::Collapsed);
    }

}
