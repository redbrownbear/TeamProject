// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/BaseUI.h"
#include "SubSystem/UI/UIManager.h"
#include "EngineUtils.h"

void UBaseUI::OnCreated()
{
}

void UBaseUI::ShowUI()
{
    PauseAllPausableActors(true);
}

void UBaseUI::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    if (!IsInViewport())
        return;

    PauseAllPausableActors(false);

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    if (UIManager)
    {
        UIManager->HideUI(UIClass);
    }
}

void UBaseUI::PauseAllPausableActors(bool bPause)
{
    UWorld* World = GetWorld();
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor && !Actor->IsA<APlayerController>())
        {
            Actor->CustomTimeDilation = bPause ? 0.0f : 1.0f;
        }
    }
}
