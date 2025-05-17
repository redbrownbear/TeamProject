// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "UI/Base/BaseUI.h"
#include "TestPopupUI.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTestPopupUI : public UBaseUI
{
	GENERATED_BODY()
	
public:
	virtual void OnCreated() override;
	void SetPopupText(FText title, FText main);

private:
	UFUNCTION()
	void OnMyButtonClicked();

private:
	void ClosePopup();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainText;
};
