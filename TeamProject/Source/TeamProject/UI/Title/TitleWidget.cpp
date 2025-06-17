// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/TitlePlayerController.h"

#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "SubSystem/UI/UIManager.h"


void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

    Continue->SetVisibility(ESlateVisibility::Hidden);
    ImgContinue->SetVisibility(ESlateVisibility::Hidden);
    ImgNewGame->SetVisibility(ESlateVisibility::Hidden);
    ImgExit->SetVisibility(ESlateVisibility::Hidden);

    if (bHasInitialized == false)
    {
        NewGame->OnClicked.AddDynamic(this, &UTitleWidget::OnStartClicked);
        bHasInitialized = true;
    }

    if (!LoadingWidgetSoftClass.IsNull())
    {
        LoadingWidgetSoftClass.LoadSynchronous();
        LoadingWidgetClass = LoadingWidgetSoftClass.Get();
    }

    check(NewGame);
}

void UTitleWidget::OnStartClicked()
{
	PlayFadeOutAndStart();
}

void UTitleWidget::OnNavigate(const FInputActionValue& InputActionValue)
{

}

void UTitleWidget::OnConfirm()
{

}

void UTitleWidget::OnFadeOutStep()
{
    float Alpha = 1.0f - (static_cast<float>(CurrentStep) / StepCount);
    SetRenderOpacity(Alpha);

    CurrentStep++;
    if (CurrentStep > StepCount)
    {
        GetWorld()->GetTimerManager().ClearTimer(FadeHandle);

        UGameInstance* GI = GetWorld()->GetGameInstance();
        if (!GI) return;

        auto* LoadingScreenSystem = GI->GetSubsystem<UGIS_ASyncLoadingScreen>();
        if (!LoadingScreenSystem) return;

        auto* PlayerManager = GI->GetSubsystem<UPlayerManager>();
        if (PlayerManager)
        {
            PlayerManager->SetLevelName(TextU(3001));
        }

        TSoftObjectPtr<UWorld> GameMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Level/GameMap.GameMap")));
        GameMap.LoadSynchronous();

        LoadingScreenSystem->OpenLevelWithLoadingScreenTitle(LoadingWidgetClass, GameMap);
    }
}

void UTitleWidget::PlayFadeOutAndStart()
{
    CurrentStep = 0;

    LoadingWidgetSoftClass.LoadSynchronous();
    LoadingWidgetClass = LoadingWidgetSoftClass.Get();

    GetWorld()->GetTimerManager().SetTimer(FadeHandle, this, &UTitleWidget::OnFadeOutStep, StepTime, true);
}
