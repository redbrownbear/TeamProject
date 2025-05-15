// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventory : public UBaseUI
{
	GENERATED_BODY()
	
public:
	virtual void OnCreated() override;

private:
	void InitUI();
	void CloseInven();


private:
	UPROPERTY()
	UUserWidget* InvenScroll;
	UPROPERTY()
	UUserWidget* InvenSlot;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;
	UPROPERTY(meta = (BindWidget))
	UImage* CoinImg;

};
