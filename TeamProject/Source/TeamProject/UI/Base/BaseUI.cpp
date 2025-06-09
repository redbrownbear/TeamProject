// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/BaseUI.h"
#include "SubSystem/UI/UIManager.h"
#include "EngineUtils.h"
#include "UI/HUD/MainHUD.h"
#include "GameFramework/PC_InGame.h"

void UBaseUI::OnCreated()
{
}

void UBaseUI::ShowUI()
{
    PauseAllPausableActors(true);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    { 
        PC_InGame->ChangeInputContext(EInputContext::IC_UI);

        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(false);
    }
}

void UBaseUI::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    if (!IsInViewport())
        return;

    PauseAllPausableActors(false);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);

        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(true);
    }

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
