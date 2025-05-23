// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

#include "Data/ItemDataRow.h"

#include "InventoryEquip.generated.h"


/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventoryEquip : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void RefreshDescription(const FItemData& ItemData);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextDescription;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemSimpleIcon;

	UPROPERTY(meta = (BindWidget))
	USizeBox* BoxItemOption;
	UPROPERTY(meta = (BindWidget))
	USizeBox* BoxItemOptionCompare;
	UPROPERTY(meta = (BindWidget))
	USizeBox* BoxItemHp;

};
