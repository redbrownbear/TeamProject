// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "UI/Shop/ShopSlot.h"

#include "Data/ShopDataRow.h"

#include "ShopScroll.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopHighlightChanged, int32, HighlightedIndex);

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShopScroll : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	void InitializePool(int32 PreloadCount);

public:
	void UpdateSlots(const TArray<FShopDataRow>& ShopList);

public:
	void MoveSelection(FIntPoint Direction);
	void SelectInit();

public:
	FItemData GetItemDataAtIndex (int32 Index) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnShopHighlightChanged OnShopHighlightChanged;

private:
	int32 CurrentIndex = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShopSlot> SlotWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ItemVerticalBox;

	UPROPERTY()
	TArray<UShopSlot*> ActiveSlots;

	UPROPERTY()
	TArray<UShopSlot*> PooledSlots;
};
