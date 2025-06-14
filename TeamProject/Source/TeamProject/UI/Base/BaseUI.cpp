// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/BaseUI.h"
#include "SubSystem/UI/UIManager.h"
#include "EngineUtils.h"
#include "Misc/SoundUtil.h"

#include "UI/HUD/MainHUD.h"
#include "GameFramework/PC_InGame.h"

void UBaseUI::OnCreated()
{
}

void UBaseUI::ShowUI()
{
    PauseAllPausableActors(true);

    SoundU(ESoundType::ESound_Confirm);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PC_InGame)
    { 
        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(false);
    }

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    if (UIManager)
    {
        UIManager->PushUI(this);
    }
}

void UBaseUI::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    if (!IsInViewport())
        return;

    SoundU(ESoundType::ESound_Cancel);

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    if (UIManager)
    {
        UIManager->HideUI(UIClass);
        UIManager->PopUI(this);

        if (UIManager->IsAnyUI())
            return;
    }

    PauseAllPausableActors(false);

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);

        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(true);
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
