// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "UI/Base/BaseUI.h"

#include "UI/Inven/Inventory.h"
#include "UI/NpcDialogue/NPCDialogue.h"

#include "UIManager.generated.h"

//����Ʈ��ȭ�� ���� ��������Ʈ
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

//---�ǵ��� ����
private:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    //UI ����
public:
    template <typename T>
    T* FindUI()
    {
        for (UBaseUI* UI : CachedUIs)
        {
            if (T* FoundUI = Cast<T>(UI))
            {
                return FoundUI;
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
//---�ǵ��� ����

public:
    UPROPERTY()
    TMap<TSubclassOf<UBaseUI>, UBaseUI*> CachedUIs;

    UPROPERTY()
    UInventory* CachedInventoryClass;
    UPROPERTY()
    UNPCDialogue* CachedDialogueClass;

public:
    UPROPERTY(BlueprintAssignable)
    FOnDialogueNextRequested OnDialogueNextRequested;
};
