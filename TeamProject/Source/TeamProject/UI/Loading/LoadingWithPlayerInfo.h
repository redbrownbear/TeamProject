// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Progressbar.h"
#include "Components/WrapBox.h"

#include "LoadingWithPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ULoadingWithPlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

	void LoadImage();
	void InitUI();

	void DrawHeart();
	void DrawStamina();


	UPROPERTY(meta = (BindWidget))
	UProgressBar* LoadingLogo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextRupeeCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTip;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTipDescription;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* HeartWrapBox;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* StaminaWrapBox;
	
	UTexture2D* ImgHeart;
	UTexture2D* ImgStamina;

	float CurrentTime = 0.f;
	float TotalTime = 3.f;
	bool bHasInitialized = false;
};
