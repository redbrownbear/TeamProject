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

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {

        PC_InGame->ChangeInputContext(EInputContext::IC_Inventory);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
    }

    BindDelegates();
}

void UQuickSlotMain::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    RemoveDelegate();
    Super::HideUI(UQuickSlotMain::StaticClass());
}

void UQuickSlotMain::InitUI()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->BindInventoryInput();
    }
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
    if (InitSlot)
    {
        InitSlot->InitFirstSlot(Parts);
    }
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
                return Other.UniqueID == Item.UniqueID;
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

    RefreshFirstSlot(ItemDataList[0].GetParts());
}
