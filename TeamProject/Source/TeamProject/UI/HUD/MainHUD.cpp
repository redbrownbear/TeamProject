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

