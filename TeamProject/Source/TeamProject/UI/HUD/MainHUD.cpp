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

void AMainHUD::ShowInteractWidget(bool bIsShow)
{
    MainWidgetInstance->ShowInteractUI(bIsShow);
}

