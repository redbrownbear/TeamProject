// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/Overlay.h"

#include "EndingCredits.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UEndingCredits : public UBaseUI
{
	GENERATED_BODY()
	
public:
	virtual void OnCreated() override;
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void InitUI();

public:
	UFUNCTION()
	virtual void OnNavigate(const FInputActionValue& Value) override {};
	UFUNCTION()
	virtual void OnConfirm(const FInputActionValue& Value) override {};
	UFUNCTION()
	virtual void OnCancel(const FInputActionValue& Value) override {};

	void PlayNextCredit();
	void PlayFade(UOverlay* Overlay, float From, float To, float Duration, TFunction<void()> OnFinished);
	void TickFadeIn();
	TFunction<void()> OnFadeInFinished;

private:
	UPROPERTY(meta = (BindWidget)) 
	UOverlay* HMHOverlay;
	UPROPERTY(meta = (BindWidget)) 
	UOverlay* SHSOverlay;
	UPROPERTY(meta = (BindWidget))
	UOverlay* LYJOverlay;
	UPROPERTY(meta = (BindWidget)) 
	UOverlay* JYHOverlay;
	UPROPERTY(meta = (BindWidget)) 
	UOverlay* ThankU;

	TArray<UOverlay*> CreditOverlays;
	int32 CurrentCreditIndex = 0;

	UOverlay* FadingOverlay = nullptr;
	FTimerHandle CreditTimerHandle;
	FTimerHandle FadeInHandle;
	int32 FadeInCurrentStep = 0;
	int32 FadeInStepCount = 0;
	float FadeInFrom = 0.0f;
	float FadeInTo = 1.0f;

	bool bHasAlreadyTriggered = false;

};
