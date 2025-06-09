// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Data/ItemDataRow.h"

#include "ShopDescription.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShopDescription : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RefreshUI(const FItemData& ItemData);

public:
	const FItemData& GetCurrentItemData() const { return CurrentItemData; }

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImg;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PriceText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoTitle;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoDescription;

private:
	UPROPERTY()
	FItemData CurrentItemData;
};
