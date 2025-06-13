// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/LoadingWithPlayerInfo.h"
#include "Misc/TextUtil.h"
#include "SubSystem/PlayerManager.h"


void ULoadingWithPlayerInfo::NativeConstruct()
{
    LoadImage();

    InitUI();
}

void ULoadingWithPlayerInfo::LoadImage()
{
    ImgHeart = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/UILoading/Loading_Heart.Loading_Heart")));
    ImgStamina = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/UILoading/Loading_Stamina.Loading_Stamina")));
}

void ULoadingWithPlayerInfo::InitUI()
{
    HeartWrapBox->ClearChildren();

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        const sPlayerStatus& PlayerStatus = PlayerManager->GetPlayerStatus();

        TextRupeeCount->SetText(FText::FromString(FString::FromInt(PlayerStatus.Rupee)));

        TextTip->SetText(FText::FromString(TextU(2001)));
        TextTipDescription->SetText(FText::FromString(TextU(2002)));

        DrawHeart();
        DrawStamina();
    }
}

void ULoadingWithPlayerInfo::DrawHeart()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        int32 MaxHP = PlayerManager->GetMaxHp();
        int32 MaxHeartCount = FMath::CeilToInt(MaxHP / 4.0f);

        for (int32 i = 0; i < MaxHeartCount; ++i)
        {
            UImage* NewHeart = NewObject<UImage>(HeartWrapBox);
            NewHeart->SetVisibility(ESlateVisibility::Visible);

            FSlateBrush Brush;
            Brush.SetResourceObject(ImgHeart);
            Brush.ImageSize = FVector2D(24.0f, 20.f);

            NewHeart->SetBrush(Brush);
            HeartWrapBox->AddChild(NewHeart);
        }
    }
}

void ULoadingWithPlayerInfo::DrawStamina()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        int32 MaxStamina = PlayerManager->GetMaxStamina();
        int32 MaxSCount = FMath::CeilToInt(MaxStamina / 50.0f);

        for (int32 i = 0; i < MaxSCount; ++i)
        {
            UImage* NewStamina = NewObject<UImage>(StaminaWrapBox);
            NewStamina->SetVisibility(ESlateVisibility::Visible);

            FSlateBrush Brush;
            Brush.SetResourceObject(ImgStamina);
            Brush.ImageSize = FVector2D(30.0f, 30.f);

            NewStamina->SetBrush(Brush);
            StaminaWrapBox->AddChild(NewStamina);
        }
    }
}
