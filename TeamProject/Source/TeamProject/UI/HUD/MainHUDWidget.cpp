// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainHUDWidget.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(HIntOverlay)
		HIntOverlay->SetVisibility(ESlateVisibility::Hidden);

	if (NameOvelay)
		NameOvelay->SetVisibility(ESlateVisibility::Hidden);

	if (StaminaImg)
	{
		DynamicMaterial = DynamicMaterial = StaminaImg->GetDynamicMaterial();
	}

	ElapsedTime = 0.0f;
	bIsFullStamina = false;
}

void UMainHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bIsFullStamina == false)
	{
		ElapsedTime += InDeltaTime;
		float FillRatio = FMath::Clamp(ElapsedTime / 5.0f, 0.0f, 1.0f);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue("Percent", FillRatio);

			if (FillRatio >= 1.0f)	
				bIsFullStamina = true;
							
			StaminaImg->SetVisibility(bIsFullStamina ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
	}
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
