// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/UIManager.h"

#include "SubSystem/UI/QuestDialogueManager.h"

void UUIManager::PostWorldInitialize()
{
    LoadUIClass();

    if (bIsCompleteDelegate)
    {
        BindDelegates();
        bIsCompleteDelegate = true;
        return;
    } 
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    bIsCompleteload = false;
}

void UUIManager::BindDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    if (QuestManager)
    {
        OnDialogueNextRequested.AddDynamic(QuestManager, &UQuestDialogueManager::HandleNextDialogueRequested);
    }
}

void UUIManager::PushUI(UBaseUI* UI)
{
    if (!UIStack.Contains(UI))
    {
        UIStack.Add(UI);
    }
}

void UUIManager::PopUI(UBaseUI* UI)
{
    UIStack.Remove(UI);
}

void UUIManager::LoadUIClass()
{
    UWorld* World = GetGameInstance()->GetWorld();

    CachedInventoryClass = CreateWidget<UInventory>(World, LoadClass<UInventory>(nullptr, TEXT("/Game/Blueprint/UI/Inventory/BP_InvenLayout.BP_InvenLayout_C")));
    if (CachedInventoryClass)
    {
        CachedInventoryClass->AddToViewport();
        CachedInventoryClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedInventoryClass->OnCreated();
        CachedUIs.Add(UInventory::StaticClass(), CachedInventoryClass);
    }

    CachedDialogueClass = CreateWidget<UNPCDialogue>(World, LoadClass<UNPCDialogue>(nullptr, TEXT("/Game/Blueprint/UI/NpcDialogue/BP_NpcDialogue.BP_NpcDialogue_C")));
    if (CachedDialogueClass)
    {
        CachedDialogueClass->AddToViewport();
        CachedDialogueClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedDialogueClass->OnCreated();
        CachedUIs.Add(UNPCDialogue::StaticClass(), CachedDialogueClass);
    }

    CachedShopClass = CreateWidget<UShop>(World, LoadClass<UShop>(nullptr, TEXT("/Game/Blueprint/UI/Shop/BP_ShopMain.BP_ShopMain_C")));
    if (CachedShopClass)
    {
        CachedShopClass->AddToViewport();
        CachedShopClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedShopClass->OnCreated();
        CachedUIs.Add(UShop::StaticClass(), CachedShopClass);
    }

    CachedQuestClass = CreateWidget<UQuest>(World, LoadClass<UQuest>(nullptr, TEXT("/Game/Blueprint/UI/Quest/BP_QuestMain.BP_QuestMain_C")));
    if (CachedQuestClass)
    {
        CachedQuestClass->AddToViewport();
        CachedQuestClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedQuestClass->OnCreated();
        CachedUIs.Add(UQuest::StaticClass(), CachedQuestClass);
    }

    CachedPopupClass = CreateWidget<UPopupGetItem>(World, LoadClass<UPopupGetItem>(nullptr, TEXT("/Game/Blueprint/UI/Popup/BP_PopupGetItem.BP_PopupGetItem_C")));
    if (CachedPopupClass)
    {
        CachedPopupClass->AddToViewport();
        CachedPopupClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedPopupClass->OnCreated();
        CachedUIs.Add(UPopupGetItem::StaticClass(), CachedPopupClass);
    }

    CachedQuickSlotClass = CreateWidget<UQuickSlotMain>(World, LoadClass<UQuickSlotMain>(nullptr, TEXT("/Game/Blueprint/UI/QuickSlot/BP_QuickSlot.BP_QuickSlot_C")));
    if (CachedQuickSlotClass)
    {
        CachedQuickSlotClass->AddToViewport();
        CachedQuickSlotClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedQuickSlotClass->OnCreated();
        CachedUIs.Add(UQuickSlotMain::StaticClass(), CachedQuickSlotClass);
    }

    CachedMainMapClass = CreateWidget<UMainMap>(World, LoadClass<UMainMap>(nullptr, TEXT("/Game/Blueprint/UI/Map/BP_MainMap.BP_MainMap_C")));
    if (CachedMainMapClass)
    {
        CachedMainMapClass->AddToViewport();
        CachedMainMapClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedMainMapClass->OnCreated();
        CachedUIs.Add(UMainMap::StaticClass(), CachedMainMapClass);
    }

    CachedPopupCountClass = CreateWidget<UPopupCountSelect>(World, LoadClass<UPopupCountSelect>(nullptr, TEXT("/Game/Blueprint/UI/Popup/BP_PopupItemCount.BP_PopupItemCount_C")));
    if (CachedPopupCountClass)
    {
        CachedPopupCountClass->AddToViewport();
        CachedPopupCountClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedPopupCountClass->OnCreated();
        CachedUIs.Add(UPopupCountSelect::StaticClass(), CachedPopupCountClass);
    }

    CachedGameOverClass = CreateWidget<UGameOverUI>(World, LoadClass<UGameOverUI>(nullptr, TEXT("/Game/Blueprint/UI/GameOver/BP_GameOver.BP_GameOver_C")));
    if (CachedGameOverClass)
    {
        CachedGameOverClass->AddToViewport();
        CachedGameOverClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedGameOverClass->OnCreated();
        CachedUIs.Add(UGameOverUI::StaticClass(), CachedGameOverClass);
    }

    CachedEndingCreditsClass = CreateWidget<UEndingCredits>(World, LoadClass<UEndingCredits>(nullptr, TEXT("/Game/Blueprint/UI/Ending/BP_EndCredits.BP_EndCredits_C")));
    if (CachedEndingCreditsClass)
    {
        CachedEndingCreditsClass->AddToViewport();
        CachedEndingCreditsClass->SetVisibility(ESlateVisibility::Collapsed);
        CachedEndingCreditsClass->OnCreated();
        CachedUIs.Add(UEndingCredits::StaticClass(), CachedEndingCreditsClass);
    }
}
