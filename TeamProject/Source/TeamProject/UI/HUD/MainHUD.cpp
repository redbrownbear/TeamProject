// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainHUD.h"
#include "MainHUDWidget.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

    if (MainWidgetClass)
    {
        MainWidgetInstance = CreateWidget<UMainHUDWidget>(GetWorld(), MainWidgetClass);
        if (MainWidgetInstance)
        {
            MainWidgetInstance->AddToViewport();
        }
    }

    UpdateHp();
}

void AMainHUD::SetMainHUDVisible(bool bIsShow)
{
    MainWidgetInstance->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMainHUD::ShowInteractWidget(bool bIsShow)
{
    MainWidgetInstance->ShowInteractUI(bIsShow);
}

void AMainHUD::ShowInteractName(bool bIsShow, FString Name)
{
    MainWidgetInstance->ShowInteractName(bIsShow, Name);

}

void AMainHUD::ShowBowAimgUI(bool bIsShow, int32 Count)
{
    MainWidgetInstance->ShowBowAimgUI(bIsShow, Count);
}

void AMainHUD::ShowAbilityAimUI(bool bIsShow)
{
    MainWidgetInstance->ShowAbilityAimUI(bIsShow);
}

void AMainHUD::UpdateHp()
{
    MainWidgetInstance->UpdateHP();
}

