// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/UIManager.h"

#include "SubSystem/UI/QuestDialogueManager.h"

void UUIManager::PostWorldInitialize()
{
    BindDelegates();
    LoadUIClass();
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UUIManager::BindDelegates()
{
    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    if (QuestManager)
    {
        OnDialogueNextRequested.AddDynamic(QuestManager, &UQuestDialogueManager::HandleNextDialogueRequested);
    }
}

void UUIManager::LoadUIClass()
{
    UWorld* World = GetGameInstance()->GetWorld();

    if (!CachedInventoryClass)
    {
        CachedInventoryClass = CreateWidget<UInventory>(World, LoadClass<UInventory>(nullptr, TEXT("/Game/Blueprint/UI/Inventory/BP_InvenLayout.BP_InvenLayout_C")));
        if (CachedInventoryClass)
        {
            CachedInventoryClass->AddToViewport();
            CachedInventoryClass->SetVisibility(ESlateVisibility::Collapsed);
            CachedInventoryClass->OnCreated();
            CachedUIs.Add(UInventory::StaticClass(), CachedInventoryClass);
        }
    }

    if (!CachedDialogueClass)
    {
        CachedDialogueClass = CreateWidget<UNPCDialogue>(World, LoadClass<UNPCDialogue>(nullptr, TEXT("/Game/Blueprint/UI/NpcDialogue/BP_NpcDialogue.BP_NpcDialogue_C")));
        if (CachedDialogueClass)
        {
            CachedDialogueClass->AddToViewport();
            CachedDialogueClass->SetVisibility(ESlateVisibility::Collapsed);
            CachedDialogueClass->OnCreated();
            CachedUIs.Add(UNPCDialogue::StaticClass(), CachedDialogueClass);
        }
    }
}
