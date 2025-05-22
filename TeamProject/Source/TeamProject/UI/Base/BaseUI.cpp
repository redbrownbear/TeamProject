// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/BaseUI.h"
#include "SubSystem/UI/UIManager.h"
#include "EngineUtils.h"

void UBaseUI::OnCreated()
{
    PauseAllPausableActors(true);
}

void UBaseUI::CloseUI()
{
    if (!IsInViewport())
        return;

    PauseAllPausableActors(false);

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        UIManager->RemoveUI(this);
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
