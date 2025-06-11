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
    if(MainWidgetInstance)
        MainWidgetInstance->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMainHUD::ShowInteractWidget(bool bIsShow)
{
    if (MainWidgetInstance)
        MainWidgetInstance->ShowInteractUI(bIsShow);
}

void AMainHUD::ShowInteractName(bool bIsShow, FString Name)
{
    if (MainWidgetInstance)
        MainWidgetInstance->ShowInteractName(bIsShow, Name);
}

void AMainHUD::ShowBowAimgUI(bool bIsShow, int32 Count)
{
    if (MainWidgetInstance)
        MainWidgetInstance->ShowBowAimgUI(bIsShow, Count);
}

void AMainHUD::ShowAbilityAimUI(bool bIsShow)
{
    if (MainWidgetInstance)
        MainWidgetInstance->ShowAbilityAimUI(bIsShow);
}

void AMainHUD::ShowBossHpUI(bool bIsShow, float Hp, float MaxHp, FString Name)
{
    if (MainWidgetInstance)
        MainWidgetInstance->ShowBossHP(bIsShow, Hp, MaxHp, Name);
}

void AMainHUD::ShowQuestOn(bool bIsComplete, FString Name)
{
    if (MainWidgetInstance)
        MainWidgetInstance->ShowQuestOn(bIsComplete, Name);
}

void AMainHUD::UpdateHp()
{
    if (MainWidgetInstance)
        MainWidgetInstance->UpdateHP();
}

