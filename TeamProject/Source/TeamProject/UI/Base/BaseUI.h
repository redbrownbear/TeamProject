// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Base/InputReceivableUI.h"

#include "BaseUI.generated.h"

/**
 *
 */
UCLASS()
class TEAMPROJECT_API UBaseUI : public UUserWidget, public IInputReceivableUI
{
	GENERATED_BODY()


public:
	virtual void OnCreated();
	virtual void ShowUI();
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass);

public:
	virtual void OnNavigate(const FInputActionValue& Value) override {}
	virtual void OnConfirm(const FInputActionValue& Value) override {}
	virtual void OnCancel(const FInputActionValue& Value) override {}

	void PauseAllPausableActors(bool bPause);
};