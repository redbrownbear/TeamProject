// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Components/Progressbar.h"

#include "UI/HUD/HUDMiniMap.h"

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

	void ShowBowAimgUI(bool bIsShow, int32 Count);
	void ShowAbilityAimUI(bool bIsShow);

	void ShowBossHP(bool bIsShow, float Hp, float MaxHp, FString Name);

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
	UPROPERTY(meta = (BindWidget))
	UOverlay* AimingBowOverlay;
	UPROPERTY(meta = (BindWidget))
	UOverlay* AimingAbilityOverlay;
	UPROPERTY(meta = (BindWidget))
	UOverlay* BossHpOverlay;

	UPROPERTY(meta = (BindWIdget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWIdget))
	UImage* StaminaImg;
	UPROPERTY(meta = (BindWIdget))
	UTextBlock* TextArrowCount;
	UPROPERTY(meta = (BindWIdget))
	UTextBlock* TextBossName;


	UPROPERTY(meta = (BindWIdget))
	UProgressBar* BossHpProgressBar;

	UPROPERTY(meta = (BindWIdget))
	UWrapBox* HpWrapBox;

private:
	UHUDMiniMap* BP_MiniMap;

public:
	UTexture2D* HpFull;
	UTexture2D* Hp075;
	UTexture2D* HpHalf;
	UTexture2D* Hp025;
	UTexture2D* HpEmpty;

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
