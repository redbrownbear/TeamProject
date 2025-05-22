// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainHUDWidget.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(HIntOverlay)
		HIntOverlay->SetVisibility(ESlateVisibility::Hidden);

}

void UMainHUDWidget::ShowInteractUI(bool bShow)
{
	if (HIntOverlay)
		HIntOverlay->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainHUDWidget::ShowInteractName(bool bIsShow, FString Name)
{
	if (NameOvelay)
		NameOvelay->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (bIsShow)
	{
		FText NametoText = FText::FromString(Name);
		NameText->SetText(NametoText);
	}
}
