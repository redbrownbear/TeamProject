// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/BaseUI.h"
#include "SubSystem/UI/UIManager.h"

void UBaseUI::OnCreated()
{

}

void UBaseUI::CloseUI()
{
    if (!IsInViewport())
        return;

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        UIManager->RemoveUI(this);
    }
}
