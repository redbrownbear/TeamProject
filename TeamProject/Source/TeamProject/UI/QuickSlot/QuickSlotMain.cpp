// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickSlot/QuickSlotMain.h"
#include "GameFramework/PC_InGame.h"
#include "Kismet/GameplayStatics.h"

void UQuickSlotMain::OnCreated()
{
	InitUI();
    InitializePool(50);
}

void UQuickSlotMain::ShowUI()
{
    Super::ShowUI();

    BindDelegates();
}

void UQuickSlotMain::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    RemoveDelegate();
    Super::HideUI(UQuickSlotMain::StaticClass());
}

void UQuickSlotMain::InitUI()
{

}

void UQuickSlotMain::BindDelegates()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnQuickSlotUpdated.AddDynamic(this, &UQuickSlotMain::RefreshItems);
    }
}

void UQuickSlotMain::RemoveDelegate()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
       PlayerManager->OnQuickSlotUpdated.RemoveDynamic(this, &UQuickSlotMain::RefreshItems);
    }
}

void UQuickSlotMain::InitializePool(int32 PreloadCount)
{
    for (int32 i = 0; i < PreloadCount; ++i)
    {
        UQuickItemSlot* PoolSlot = CreateWidget<UQuickItemSlot>(GetWorld(), SlotWidgetClass);
        PoolSlot->RemoveFromParent();
        PooledSlots.Add(PoolSlot);
    }
}

void UQuickSlotMain::RefreshFirstSlot(eEquipParts Parts)
{
    CurrentPart = Parts;
    if (InitSlot)
    {
        InitSlot->InitFirstSlot(Parts);
    }
}

void UQuickSlotMain::MoveSelection(FIntPoint Direction)
{
    if (ActiveSlots.Num() == 0) return;

    const int32 MaxIndex = ActiveSlots.Num() -1;

    int32 NextIndex = CurrentIndex;

    if (Direction.X != 0) // 좌우
    {
        NextIndex += Direction.X;
    }

    NextIndex = FMath::Clamp(NextIndex, 0, MaxIndex);

    if (NextIndex != CurrentIndex)
    {
        ActiveSlots[CurrentIndex]->SetSelected(false);
        ActiveSlots[NextIndex]->SetSelected(true);
        CurrentIndex = NextIndex;
    }

    FItemData Itemdata = ActiveSlots[CurrentIndex]->GetItemData();
}

void UQuickSlotMain::EquipCrrentItem()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    if (ActiveSlots.IsEmpty())
        return;

    for (UQuickItemSlot* slot : ActiveSlots)
    {
        FItemData Item = PlayerManager->GetItemByUniqueID(slot->GetItemData().UniqueID);
        if (PlayerManager->IsEquipPart(Item.GetParts()) && Item.UniqueID == slot->GetItemData().UniqueID)
        {
            slot->SetEquiped(true);
        }
        else
        {
            slot->SetEquiped(false);
        }
    }
}

void UQuickSlotMain::OnNavigate(const FInputActionValue& InputActionValue)
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
            MoveSelection(FIntPoint(1, 0));
        else
            MoveSelection(FIntPoint(-1, 0));
    }
}


void UQuickSlotMain::OnCancel(const FInputActionValue& InputActionValue)
{
    if (!ActiveSlots.IsEmpty())
    {
        ActiveSlots[CurrentIndex]->SetEquipDate();
    }
        
    HideUI(UQuickSlotMain::StaticClass());
}

void UQuickSlotMain::RefreshItems(const TArray<FItemData>& ItemDataList)
{
    if (!SlotWidgetClass)
        return;

    for (UQuickItemSlot* ActiveSlot : ActiveSlots)
    {
        ActiveSlot->RemoveFromParent();
        PooledSlots.Add(ActiveSlot);
    }
    ActiveSlots.Empty();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    TArray<FItemData> TempItems = PlayerManager->GetAllItemData();

    TArray<FItemData> Items;
    for (FItemData Item : TempItems)
    {
        if ((Item.eItemCategory == EItemCategory::IT_Arrow && Item.bIsArrow) || Item.eItemCategory == EItemCategory::IT_Material)
        {
            // 이미 동일한 ItemCode가 있는지 확인
            FItemData* Found = Items.FindByPredicate([&](const FItemData& Other) {
                return Other.Name == Item.Name;
                });

            if (Found)
                Found->ItemCount += Item.ItemCount;

            else
                Items.Add(Item);

        }

        else
            Items.Add(Item);
    }

    if (Items.IsEmpty())
        return;

    Items.Sort([](const FItemData& A, const FItemData& B)
        {
            if (A.eItemCategory == B.eItemCategory)
            {
                if (A.eArmorKind == B.eArmorKind)
                    return A.Damage > B.Damage;

                return A.eArmorKind < B.eArmorKind;
            }
            return A.eItemCategory < B.eItemCategory;
        });

    for (FItemData Item : Items)
    {
        if (Item.GetParts() != CurrentPart)
            continue;

        UQuickItemSlot* NewSlot = nullptr;
        if (PooledSlots.Num() > 0)
        {
            NewSlot = PooledSlots.Pop();
        }
        else
        {
            NewSlot = CreateWidget<UQuickItemSlot>(GetWorld(), SlotWidgetClass);
        }

        NewSlot->SetItemData(Item);
        ItemWrapBox->AddChildToWrapBox(NewSlot);
        ActiveSlots.Add(NewSlot);
    }

    if (ActiveSlots.IsEmpty())
        return;

    CurrentIndex = 0;

    EquipCrrentItem();
    ActiveSlots[CurrentIndex]->SetSelected(true);
}
