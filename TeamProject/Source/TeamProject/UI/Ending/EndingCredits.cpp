// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Ending/EndingCredits.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PC_InGame.h"
#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"

void UEndingCredits::OnCreated()
{
    InitUI();
}

void UEndingCredits::ShowUI()
{
    Super::ShowUI();

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_UI);

        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC_InGame->SetInputMode(InputMode);
        PC_InGame->bShowMouseCursor = true;
    }

    bIsFocusable = true;
    SetKeyboardFocus();

    CreditOverlays = { HMHOverlay, SHSOverlay, LYJOverlay, JYHOverlay, ThankU };

    for (UOverlay* Overlay : CreditOverlays)
    {
        if (Overlay)
        {
            Overlay->SetRenderOpacity(0.0f);
            Overlay->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    PlayNextCredit();
}

void UEndingCredits::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    Super::HideUI(UEndingCredits::StaticClass());

    UGIS_ASyncLoadingScreen* LoadingScreenSubsystem = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
    if (LoadingScreenSubsystem)
    {
        TSoftObjectPtr<UWorld> GameMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Level/TitleMap.TitleMap")));
        LoadingScreenSubsystem->OpenLevelWithLoadingScreenGameOver(GameMap);
    }
}

FReply UEndingCredits::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (!bHasAlreadyTriggered)
    {
        bHasAlreadyTriggered = true;

        HideUI(UEndingCredits::StaticClass());
    }

    return FReply::Handled();
}

void UEndingCredits::InitUI()
{
    bHasAlreadyTriggered = true;
}

void UEndingCredits::PlayNextCredit()
{
    if (CurrentCreditIndex >= CreditOverlays.Num())
    {
        bHasAlreadyTriggered = false;
        return;
    }

    UOverlay* CurrentOverlay = CreditOverlays[CurrentCreditIndex];
    if (!CurrentOverlay) return;

    CurrentOverlay->SetVisibility(ESlateVisibility::Visible);

    PlayFade(CurrentOverlay, 0.0f, 1.0f, 1.0f, [this, CurrentOverlay]()
        {
            GetWorld()->GetTimerManager().SetTimer(CreditTimerHandle, [this, CurrentOverlay]()
                {
                    PlayFade(CurrentOverlay, 1.0f, 0.0f, 1.0f, [this, CurrentOverlay]()
                        {
                            CurrentOverlay->SetVisibility(ESlateVisibility::Hidden);
                            CurrentCreditIndex++;
                            PlayNextCredit();
                        });
                }, 3.0f, false);
        });
}

void UEndingCredits::PlayFade(UOverlay* Overlay, float From, float To, float Duration, TFunction<void()> OnFinished)
{
    if (!Overlay) return;

    // ÁßÃ¸ ¹æÁö
    if (GetWorld()->GetTimerManager().IsTimerActive(FadeInHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(FadeInHandle);
    }

    FadingOverlay = Overlay;
    FadeInFrom = From;
    FadeInTo = To;
    FadeInCurrentStep = 0;
    FadeInStepCount = FMath::Max(1, FMath::RoundToInt(Duration / 0.01f));
    OnFadeInFinished = MoveTemp(OnFinished);

    GetWorld()->GetTimerManager().SetTimer(FadeInHandle, this, &UEndingCredits::TickFadeIn, 0.01f, true);
}

void UEndingCredits::TickFadeIn()
{
    if (!IsValid(FadingOverlay))
    {
        GetWorld()->GetTimerManager().ClearTimer(FadeInHandle);
        return;
    }

    float Alpha = FMath::Lerp(FadeInFrom, FadeInTo, static_cast<float>(FadeInCurrentStep) / FadeInStepCount);
    FadingOverlay->SetRenderOpacity(Alpha);

    FadeInCurrentStep++;
    if (FadeInCurrentStep >= FadeInStepCount)
    {
        GetWorld()->GetTimerManager().ClearTimer(FadeInHandle);
        if (OnFadeInFinished)
        {
            OnFadeInFinished();
        }
    }
}
