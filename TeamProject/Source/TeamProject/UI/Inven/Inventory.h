// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"
#include "UI/Inven/InventoryScroll.h"

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
	void RefreshInventory(FItemData ItemData);


private:
	UPROPERTY(meta = (BindWidget))
	UInventoryScroll* InvenScroll;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;
	UPROPERTY(meta = (BindWidget))
	UImage* CoinImg;

};
