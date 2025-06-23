// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopSellScroll.h"

#include "SubSystem/PlayerManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "GameFramework/PC_InGame.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Npc/Npc.h"

void UShopSellScroll::NativeConstruct()
{
    Super::NativeConstruct();

	if (bHasInitialized == false)
	{
        InitCategory();
        InitializePool(100);
		bHasInitialized = true;
	}

    check(ItemWrapBox);
}

void UShopSellScroll::InitializePool(int32 PreloadCount)
{
    for (int32 i = 0; i < PreloadCount; ++i)
    {
        UShopSellSlot* PoolSlot = CreateWidget<UShopSellSlot>(GetWorld(), SlotWidgetClass);
        PoolSlot->RemoveFromParent();
        PooledSlots.Add(PoolSlot);
    }
}

void UShopSellScroll::AddItemSlot(const FItemData& NewItem)
{
    SelectCategory(NewItem.eItemCategory, true);
}

void UShopSellScroll::UpdateSlots(const TArray<FItemData>& NewItemList)
{
    for (UShopSellSlot* ActiveSlot : ActiveSlots)
    {
        ActiveSlot->RemoveFromParent();
        PooledSlots.Add(ActiveSlot);
    }

    ActiveSlots.Empty();

    for (const FItemData& Item : NewItemList)
    {
        if (Item.eItemCategory != CurrentCategory)
            continue;

        UShopSellSlot* NewSlot = nullptr;

        if (PooledSlots.Num() > 0)
        {
            NewSlot = PooledSlots.Pop();
        }
        else
        {
            NewSlot = CreateWidget<UShopSellSlot>(GetWorld(), SlotWidgetClass);
        }

        NewSlot->SetItemData(Item);
        ItemWrapBox->AddChildToWrapBox(NewSlot);
        ActiveSlots.Add(NewSlot);
    }

    SelectCategory(EItemCategory::IT_Weapon, true);
}

void UShopSellScroll::MoveSelection(FIntPoint Direction)
{
    if (ActiveSlots.Num() == 0) return;

    const int32 NumPerRow = 5;
    const int32 MaxIndex = ActiveSlots.Num() - 1;

    int32 NextIndex = CurrentIndex;

    if (Direction.X != 0)
    {
        NextIndex += Direction.X;
    }
    else if (Direction.Y != 0)
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

    if (UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>())
    {
        ShopManager->SetSelectedItem(Itemdata);
    }

    OnShopDescriptionUpdated.Broadcast(Itemdata);
}

void UShopSellScroll::InitSelectItem()
{
    if (ActiveSlots.IsEmpty())
    {
        OnShopDescriptionInitUpdated.Broadcast();
        return;
    }
        

    for (UShopSellSlot* slot : ActiveSlots)
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

    OnShopDescriptionUpdated.Broadcast(Itemdata);
}

void UShopSellScroll::MoveCategory(bool IsLeft)
{
    EItemCategory Category = GetNextCategory(CurrentCategory, IsLeft);

    SelectCategory(Category, true);
}

void UShopSellScroll::ItemSell()
{
    FItemData CurItemData = GetCurItem();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    UShopManager* ShopManager = GetGameInstance()->GetSubsystem<UShopManager>();
    check(ShopManager);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    check(PC_InGame);

    int32 Rupee = PlayerManager->GetRupee() + CurItemData.price;
    PlayerManager->SetRupee(Rupee);
    PlayerManager->RemoveItemByUniqueID(CurItemData.UniqueID);

    TArray<FShopDataRow> CurrentShopDataArray = ShopManager->GetShopData(PC_InGame->Npc->GetQuestCharacterType());
    FShopDataRow ShopDataRow;
    int iShopDataIndex = -1;

    for (int32 i = 0; i < CurrentShopDataArray.Num(); ++i)
    {
        if (CurrentShopDataArray[i].ItemData.ItemCode == CurItemData.ItemCode)
        {
            CurrentShopDataArray[i].ItemData.ItemCount++;
            iShopDataIndex = i;
            ShopManager->UpdateShopData(PC_InGame->Npc->GetQuestCharacterType(), CurrentShopDataArray[iShopDataIndex]);
            break;
        }
    }

    if (iShopDataIndex == -1)
    {
        ShopDataRow.QuestCharacter = PC_InGame->Npc->GetQuestCharacterType();
        ShopDataRow.ItemData = CurItemData;
        ShopDataRow.InitialItemCount = CurItemData.ItemCount;
        CurrentShopDataArray.Add(ShopDataRow);

        ShopManager->UpdateShopData(PC_InGame->Npc->GetQuestCharacterType(), ShopDataRow);
    }

    PC_InGame->Npc->SetCurrentDialogueType(EDialogType::SuccessfulSale);
}

EItemCategory UShopSellScroll::GetNextCategory(EItemCategory Current, bool bIsLeft)
{
    int32 CategoryCount = static_cast<int32>(EItemCategory::IT_END);
    int32 Index = static_cast<int32>(Current) + (bIsLeft ? -1 : 1);

    if (Index < 0)
        Index = CategoryCount - 1;

    if (Index >= CategoryCount)
        Index = 0;

    return static_cast<EItemCategory>(Index);
}

const TArray<FItemData>& UShopSellScroll::GetSellableItems()
{
    SellableItems.Reset();

    for (UShopSellSlot* ActiveSlot : ActiveSlots)
    {
        SellableItems.Add(ActiveSlot->GetItemData());
    }

    return SellableItems;
}

void UShopSellScroll::SetSort(EItemCategory Type)
{
    if (!SlotWidgetClass)
        return;

    for (UShopSellSlot* ActiveSlot : ActiveSlots)
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
            FItemData* Found = Items.FindByPredicate([&](const FItemData& Other) {
                return Other.ItemCode == Item.ItemCode;
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
    {
        OnShopDescriptionInitUpdated.Broadcast();
        return;
    }
        
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

        UShopSellSlot* NewSlot = nullptr;
        if (PooledSlots.Num() > 0)
        {
            NewSlot = PooledSlots.Pop();
        }
        else
        {
            NewSlot = CreateWidget<UShopSellSlot>(GetWorld(), SlotWidgetClass);
        }

        NewSlot->SetItemData(Item);
        ItemWrapBox->AddChildToWrapBox(NewSlot);
        ActiveSlots.Add(NewSlot);
    }

    InitSelectItem();
}

void UShopSellScroll::InitCategory()
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

    WeaponCheck->OnCheckStateChanged.AddDynamic(this, &UShopSellScroll::OnWeaponCheckChanged);
    ArrowCheck->OnCheckStateChanged.AddDynamic(this, &UShopSellScroll::OnArrowCheckChanged);
    ShieldCheck->OnCheckStateChanged.AddDynamic(this, &UShopSellScroll::OnShieldCheckChanged);
    ArmorCheck->OnCheckStateChanged.AddDynamic(this, &UShopSellScroll::OnArmorCheckChanged);
    MaterialCheck->OnCheckStateChanged.AddDynamic(this, &UShopSellScroll::OnMaterialCheckChanged);
    FoodCheck->OnCheckStateChanged.AddDynamic(this, &UShopSellScroll::OnFoodCheckChanged);
}


void UShopSellScroll::SelectCategory(EItemCategory type, bool bIsChecked)
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
