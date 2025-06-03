// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/InventoryScroll.h"

#include "SubSystem/PlayerManager.h"

void UInventoryScroll::NativeConstruct()
{
    Super::NativeConstruct();

    InitCategory();

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
    SelectCategory(NewItem.eItemCategory, true);
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
        if (Item.eItemCategory != CurrentCategory)
            continue;

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

    InitSelectItem();
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
        ScrollBox->ScrollWidgetIntoView(ActiveSlots[NextIndex], true, EDescendantScrollDestination::IntoView);
        ActiveSlots[CurrentIndex]->SetSelected(false);
        ActiveSlots[NextIndex]->SetSelected(true);
        CurrentIndex = NextIndex;
    }

    FItemData Itemdata = ActiveSlots[CurrentIndex]->GetItemData();
    OnInventoryDescriptionUpdated.Broadcast(Itemdata); // UI에게 알림
}

void UInventoryScroll::InitSelectItem()
{
    if (ActiveSlots.IsEmpty())
        return;

    for (UInventorySlot* slot : ActiveSlots)
    {
        slot->SetSelected(false);
    }

    CurrentIndex = 0;
    ActiveSlots[CurrentIndex]->SetSelected(true);

    FItemData Itemdata = ActiveSlots[CurrentIndex]->GetItemData();
    OnInventoryDescriptionUpdated.Broadcast(Itemdata); // UI에게 알림
}

FVector2D UInventoryScroll::GetItemListLocation()
{
    FGeometry Geometry = ActiveSlots[CurrentIndex]->GetCachedGeometry();

    FVector2D RawPos = Geometry.GetAbsolutePosition();
    float Scale = Geometry.GetAccumulatedLayoutTransform().GetScale();

    FVector2D AdjustedPos = RawPos / Scale;
    return  AdjustedPos;
}

void UInventoryScroll::SetSort(EItemCategory Type)
{
    if (!SlotWidgetClass)
        return;

    for (UInventorySlot* ActiveSlot : ActiveSlots)
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
                return Other.ItemID == Item.ItemID;
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
        if (Item.eItemCategory != CurrentCategory)
            continue;

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

    InitSelectItem();
}

void UInventoryScroll::InitCategory()
{
    MapCategory.Add(EItemCategory::IT_Weapon, WeaponCheck);
    MapCategory.Add(EItemCategory::IT_Arrow, ArrowCheck);
    MapCategory.Add(EItemCategory::IT_Shield, ShieldCheck);
    MapCategory.Add(EItemCategory::IT_Armor, ArmorCheck);
    MapCategory.Add(EItemCategory::IT_Material, MaterialCheck);
    MapCategory.Add(EItemCategory::IT_Food, FoodCheck);

    MapCategoryText.Add(EItemCategory::IT_Weapon, WeaponText);
    MapCategoryText.Add(EItemCategory::IT_Arrow, ArrowText);
    MapCategoryText.Add(EItemCategory::IT_Shield, ShieldText);
    MapCategoryText.Add(EItemCategory::IT_Armor, ArmorText);
    MapCategoryText.Add(EItemCategory::IT_Material, MaterialText);
    MapCategoryText.Add(EItemCategory::IT_Food, FoodText);

    WeaponCheck->OnCheckStateChanged.AddDynamic(this, &UInventoryScroll::OnWeaponCheckChanged);
    ArrowCheck->OnCheckStateChanged.AddDynamic(this, &UInventoryScroll::OnArrowCheckChanged);
    ShieldCheck->OnCheckStateChanged.AddDynamic(this, &UInventoryScroll::OnShieldCheckChanged);
    ArmorCheck->OnCheckStateChanged.AddDynamic(this, &UInventoryScroll::OnArmorCheckChanged);
    MaterialCheck->OnCheckStateChanged.AddDynamic(this, &UInventoryScroll::OnMaterialCheckChanged);
    FoodCheck->OnCheckStateChanged.AddDynamic(this, &UInventoryScroll::OnFoodCheckChanged);

    SelectCategory(EItemCategory::IT_Weapon, true);
}


void UInventoryScroll::SelectCategory(EItemCategory type, bool bIsChecked)
{
    if (!bIsChecked)
    {
        MapCategory[CurrentCategory]->SetIsChecked(true);
        return;
    }
        
    for (const TPair<EItemCategory, UCheckBox*>& Pair : MapCategory)
    {
        UCheckBox* CheckBox = Pair.Value;

        if (!CheckBox)
            continue;
       
        CheckBox->SetIsChecked(false);    
    }

    for (const TPair<EItemCategory, UTextBlock*>& Pair : MapCategoryText)
    {
        UTextBlock* TextBlock = Pair.Value;

        if (!TextBlock)
            continue;

        TextBlock->SetVisibility(ESlateVisibility::Hidden);
    }

    MapCategory[type]->SetIsChecked(bIsChecked);
    MapCategoryText[type]->SetVisibility(ESlateVisibility::Visible);

    CurrentCategory = type;

    SetSort(CurrentCategory);
}
