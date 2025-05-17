// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Data/ItemDataRow.h"

#include "UI/Inven/InventorySlot.h"

#include "InventoryScroll.generated.h"


/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventoryScroll : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	void InitializePool(int32 PreloadCount);


public:
	void AddItemSlot(const FItemData& NewItem);
	void UpdateSlots(const TArray<FItemData>& NewItemList);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlot> SlotWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* ItemWrapBox;

	UPROPERTY()
	TArray<UInventorySlot*> ActiveSlots;

	UPROPERTY()
	TArray<UInventorySlot*> PooledSlots;


};
