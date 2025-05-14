// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UI/Base/BaseUI.h"
#include "UI/Test/TestPopupUI.h"

#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

//UI 생성
public:
    UTestPopupUI* CreatePopup(UWorld* World);

//UI 제거
public:
    void RemovePopup(UTestPopupUI* PopupUI);

//---건들지 말기
private:
    template <typename T>
    T* CreateUI(UWorld* World, TSubclassOf<UBaseUI> UIClass)
    {
        if (!World || !UIClass)
            return nullptr;

        T* NewUI = CreateWidget<T>(World, UIClass);
        if (NewUI)
        {
            NewUI->AddToViewport();
            NewUI->OnCreated();
            CreatedUIs.Add(NewUI);
        }
        return NewUI;
    };

    void RemoveUI(UBaseUI* TargetUI)
    {
        if (!TargetUI) return;

        for (int32 i = 0; i < CreatedUIs.Num(); ++i)
        {
            if (!IsValid(CreatedUIs[i]))
                continue;

            if (CreatedUIs[i]->GetClass() == TargetUI->GetClass())
            {
                TargetUI->RemoveFromParent();
                CreatedUIs.RemoveAt(i);
                break;
            }
        }
    }

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
//---건들지 말기


private:
    UPROPERTY()
    TArray<UBaseUI*> CreatedUIs;

    //UI모임
    UPROPERTY()
    TSubclassOf<UTestPopupUI> PopupUIBPClass;
};
