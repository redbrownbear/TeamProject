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

	check(NewGame);

	NewGame->OnClicked.AddDynamic(this, &UTitleWidget::OnStartClicked);

	ImgContinue->SetVisibility(ESlateVisibility::Visible);
	ImgNewGame->SetVisibility(ESlateVisibility::Hidden);
	ImgExit->SetVisibility(ESlateVisibility::Hidden);
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

void UTitleWidget::PlayFadeOutAndStart()
{
    CurrentStep = 0;
    const int StepCount = 30;
    const float StepTime = 0.033f;

    TSoftObjectPtr<UWorld> GameMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Level/GameMap.GameMap")));
    GetWorld()->GetTimerManager().SetTimer(FadeHandle, FTimerDelegate::CreateLambda([this, StepCount, GameMap]()
        {
            float Alpha = 1.0f - (static_cast<float>(CurrentStep) / StepCount);
            this->SetRenderOpacity(Alpha);

            CurrentStep++;
            if (CurrentStep > StepCount)
            {
                GetWorld()->GetTimerManager().ClearTimer(FadeHandle);

                UGIS_ASyncLoadingScreen* LoadingScreenSystem = GetWorld()->GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
                if (LoadingScreenSystem)
                {
                    LoadingScreenSystem->OpenLevelWithLoadingScreenTitle(LoadingWidgetClass, GameMap);
                }
            }
        }), StepTime, true);

}
