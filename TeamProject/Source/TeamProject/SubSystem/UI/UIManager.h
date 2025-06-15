// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "UI/Base/BaseUI.h"

#include "UI/Inven/Inventory.h"
#include "UI/NpcDialogue/NPCDialogue.h"
#include "UI/Shop/Shop.h"
#include "UI/Quest/Quest.h"
#include "UI/Popup/PopupGetItem.h"
#include "UI/QuickSlot/QuickSlotMain.h"
#include "UI/Map/MainMap.h"
#include "UI/Popup/PopupCountSelect.h"
#include "UI/GameOver/GameOverUI.h"
#include "UI/Ending/EndingCredits.h"

#include "UIManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueNextRequested, EQuestCharacter, QuestChar, int32, DialogueID);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TEAMPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    void PostWorldInitialize();
    void BindDelegates();
    void LoadUIClass();

private:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    //UI
public:
    template <typename T>
    T* FindUI()
    {
        for (auto& Pair : CachedUIs)
        {
            if (T* FoundUI = Cast<T>(Pair.Value))
            {
                return FoundUI;
            }
        }
        return nullptr;
    }

    template <typename T>
    T* FindTopUI()
    {
        for (int32 i = UIStack.Num() - 1; i >= 0; --i)
        {
            if (T* UI = Cast<T>(UIStack[i]))
            {
                if (UI->IsInViewport() && UI->IsVisible())
                    return UI;
            }
        }
        return nullptr;
    }

    void ShowUI(TSubclassOf<UBaseUI> UIClass)
    {
        if (!UIClass) return;

        UBaseUI* FoundUI = nullptr;
        if (CachedUIs.Contains(UIClass))
        {
            FoundUI = CachedUIs[UIClass];
            if (IsValid(FoundUI))
            {
                FoundUI->SetVisibility(ESlateVisibility::Visible);
                FoundUI->ShowUI();
            }
        }
    }

    void HideUI(TSubclassOf<UBaseUI> UIClass)
    {
        if (!UIClass) return;

        UBaseUI* FoundUI = nullptr;
        if (CachedUIs.Contains(UIClass))
        {
            FoundUI = CachedUIs[UIClass];
            if (IsValid(FoundUI))
            {
                (FoundUI)->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    };

public:
    void PushUI(UBaseUI* UI);
    void PopUI(UBaseUI* UI);
    
    bool IsAnyUI() { return !UIStack.IsEmpty(); }

private:
    UPROPERTY()
    TMap<TSubclassOf<UBaseUI>, UBaseUI*> CachedUIs;

    UPROPERTY()
    TArray<UBaseUI*> UIStack;

    UPROPERTY()
    UInventory* CachedInventoryClass;
    UPROPERTY()
    UNPCDialogue* CachedDialogueClass;
    UPROPERTY()
    UShop* CachedShopClass;
    UPROPERTY()
    UQuest* CachedQuestClass;
    UPROPERTY()
    UPopupGetItem* CachedPopupClass;
    UPROPERTY()
    UQuickSlotMain* CachedQuickSlotClass;
    UPROPERTY()
    UMainMap* CachedMainMapClass;
    UPROPERTY()
    UPopupCountSelect* CachedPopupCountClass;
    UPROPERTY()
    UGameOverUI* CachedGameOverClass;
    UPROPERTY()
    UEndingCredits* CachedEndingCreditsClass;


public:
    UPROPERTY(BlueprintAssignable)
    FOnDialogueNextRequested OnDialogueNextRequested;

private:
    bool bIsCompleteload;
    bool bIsCompleteDelegate = false;
};
