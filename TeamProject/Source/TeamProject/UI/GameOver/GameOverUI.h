// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UGameOverUI : public UBaseUI
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

private:
	void FadeInThis();
	void StartImageFade();
	void FadeInImage();
	void ShowPressAnyKeyText();

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ImageGameOver;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPressAnyKey;

private:
	FTimerHandle FadeThisHandle;
	FTimerHandle ImageFadeHandle;
	FTimerHandle ImageFadeTickHandle;
	FTimerHandle ShowTextHandle;

	bool bHasAlreadyTriggered = false;
};
