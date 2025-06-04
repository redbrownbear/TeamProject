// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Data/ItemDataRow.h"

#include "InventorySlot.generated.h"


/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void SetItemData(const FItemData& Item);
	const FItemData& GetItemData() const { return ItemData; }

public:
	void SetSelected(bool bIsSelected);
	void SetEquiped(bool bIsEquiped);
private:
	FItemData ItemData;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
	UPROPERTY(meta = (BindWidget))
	UImage* HighlightBg;
	UPROPERTY(meta = (BindWidget))
	UImage* EquipedBg;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* Amount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Value;
};
