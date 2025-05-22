// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "UI/Base/BaseUI.h"

#include "UIManager.generated.h"

//퀘스트대화를 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueNextRequested, EQuestCharacter, QuestChar, int32, DialogueID);

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()


//---건들지 말기
private:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    //UI 생성
public:
    template <typename T>
    T* CreateUI(UWorld* World, TSubclassOf<T> UIClass)
    {
        if (!World || !UIClass)
            return nullptr;

        for (UBaseUI* UI : CreatedUIs)
        {
            if (UI && UI->IsA(UIClass))
            {
                return nullptr;
            }
        }

        T* NewUI = CreateWidget<T>(World, UIClass);
        if (NewUI)
        {
            NewUI->AddToViewport();
            NewUI->OnCreated();
            CreatedUIs.Add(NewUI);
        }
        return NewUI;
    }

    template <typename T>
    T* FindUI()
    {
        for (UBaseUI* UI : CreatedUIs)
        {
            if (T* FoundUI = Cast<T>(UI))
            {
                return FoundUI;
            }
        }
        return nullptr;
    }

    void RemoveUI(UBaseUI* TargetUI)
    {
        if (!TargetUI) return;

        for (int32 i = 0; i < CreatedUIs.Num(); ++i)
        {
            if (!IsValid(CreatedUIs[i]))
                continue;

            if (CreatedUIs[i] == TargetUI)
            {
                TargetUI->RemoveFromParent();
                CreatedUIs.RemoveAt(i);
                break;
            }
        }
    };
//---건들지 말기

private:
    UPROPERTY()
    TArray<UBaseUI*> CreatedUIs;

public:
    UPROPERTY(BlueprintAssignable)
    FOnDialogueNextRequested OnDialogueNextRequested;
};
