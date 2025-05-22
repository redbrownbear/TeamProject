// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

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
	void ShowInteractName(bool bIsShow, FString Name);

public:
	UPROPERTY(meta = (BindWidget))
	UOverlay* HIntOverlay;

	UPROPERTY(meta = (BindWidget))
	UOverlay* NameOvelay;
	UPROPERTY(meta = (BindWIdget))
	UTextBlock* NameText;
};
