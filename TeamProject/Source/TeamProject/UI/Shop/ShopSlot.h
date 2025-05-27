// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/ItemDataRow.h"

#include "ShopSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShopSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetItemData(const FItemData& Item);
	void SetSelected(bool bIsSelected);

public:
	const FItemData& GetItemData() const { return ItemData; }

private:
	FItemData ItemData;

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImg;

	UPROPERTY(meta = (BindWidget))
	UImage* HighlightBg;
	UPROPERTY(meta = (BindWidget))
	UImage* NoneBg;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemPrice;
	
};
