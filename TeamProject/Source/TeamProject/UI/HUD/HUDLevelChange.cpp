// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/HUDLevelChange.h"
#include "SubSystem/PlayerManager.h"

void UHUDLevelChange::NativeConstruct()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        ShowLevelChangeUI(PlayerManager->GetLevelName());
        UpdateCanTick();
    }
}

void UHUDLevelChange::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bOepnLevel == false)
        return;

    FadeTime += InDeltaTime;

    float Alpha = FMath::Clamp(FadeTime / 1.0f, 0.f, 1.f);

    if (TextLevel)
    {
        TextLevel->SetRenderOpacity(bFadingIn ? Alpha : (1.0f - Alpha));
    }

    if (FadeTime >= 3.0f)
    {
        if (!bFadingIn)
        {
            bOepnLevel = false;
            SetVisibility(ESlateVisibility::Hidden); // 필요하면 UI를 숨기기
        }

        bFadingIn = !bFadingIn;
        FadeTime = 0.f;
    }
}

void UHUDLevelChange::ShowLevelChangeUI(FString LevelName)
{
    SetVisibility(ESlateVisibility::Visible);

    TextLevel->SetText(FText::FromString(LevelName));
    bOepnLevel = true;
}
