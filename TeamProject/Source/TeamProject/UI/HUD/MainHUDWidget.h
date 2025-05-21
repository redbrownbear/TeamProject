// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"

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

public:
	void ShowInteractUI(bool bIsShow);

public:
	UPROPERTY(meta = (BindWidget))
	UOverlay* HIntOverLay;
};
