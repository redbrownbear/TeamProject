// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"

#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void InitValue();
	void InitResource();

public:
	void ShowInteractUI(bool bIsShow);
	void ShowInteractName(bool bIsShow, FString Name);

public:
	void UpdateHP();
	void UpdateStamina(float InDeltaTime);

private:
	void HideStamina();

public:
	UPROPERTY(meta = (BindWidget))
	UOverlay* HIntOverlay;
	UPROPERTY(meta = (BindWidget))
	UOverlay* NameOvelay;
	UPROPERTY(meta = (BindWIdget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWIdget))
	UImage* StaminaImg;

public:
	UTexture2D* HpFull;
	UTexture2D* Hp075;
	UTexture2D* HpHalf;
	UTexture2D* Hp025;
	UTexture2D* HpEmpty;

	UPROPERTY(meta = (BindWIdget))
	UWrapBox* HpWrapBox;

	UPROPERTY()
	TArray<UImage*> HeartImages;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	FTimerHandle StaminaHideTimerHandle;

	float ElapsedTime;
	float StaminaHideDelayTime;
	float StaminaRegenSpeed;
	bool bIsStaminaVisible;

};
