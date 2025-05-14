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

//UI ����
public:
    UTestPopupUI* CreatePopup(UWorld* World);

//UI ����
public:
    void RemovePopup(UTestPopupUI* PopupUI);

//---�ǵ��� ����
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
//---�ǵ��� ����


private:
    UPROPERTY()
    TArray<UBaseUI*> CreatedUIs;

    //UI����
    UPROPERTY()
    TSubclassOf<UTestPopupUI> PopupUIBPClass;
};
