// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/ItemDataRow.h"

#include "UI/Inven/InventoryScroll.h"

#include "UI/Base/BaseUI.h"
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
	UFUNCTION()
	void RefreshInventory(const FItemData& ItemData);


private:
	UPROPERTY(meta = (BindWidget))
	UInventoryScroll* BP_InvenScroll;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;
	UPROPERTY(meta = (BindWidget))
	UImage* CoinImg;

};
