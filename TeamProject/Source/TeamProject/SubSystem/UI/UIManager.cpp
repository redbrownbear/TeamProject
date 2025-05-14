// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/UIManager.h"

UTestPopupUI* UUIManager::CreatePopup(UWorld* World)
{
    for (UBaseUI* UI : CreatedUIs)
    {
        if (UI && UI->IsA(PopupUIBPClass))
        {
            return nullptr;
        }
    }
       
    return CreateUI<UTestPopupUI>(World, PopupUIBPClass);
}

void UUIManager::RemovePopup(UTestPopupUI* PopupUI)
{
    RemoveUI(PopupUI);
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    FString Path = TEXT("/Game/UIBluePrint/BP_TestPopup.BP_TestPopup_C");
    PopupUIBPClass = LoadClass<UBaseUI>(nullptr, *Path);
}