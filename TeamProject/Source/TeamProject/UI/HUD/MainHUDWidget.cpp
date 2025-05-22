// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainHUDWidget.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(HIntOverLay)
		HIntOverLay->SetVisibility(ESlateVisibility::Hidden);

}

void UMainHUDWidget::ShowInteractUI(bool bShow)
{
	if (HIntOverLay)
		HIntOverLay->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
