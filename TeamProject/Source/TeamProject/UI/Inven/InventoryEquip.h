// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

#include "Data/ItemDataRow.h"
#include "SubSystem/PlayerManager.h"

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

	void SetEquipMakeData(const TArray<FItemData>& ItemDataList);

	void RefreshEquip(const TMap<eEquipParts, FItemData>& ItemDataMap);



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


	UPROPERTY(meta = (BindWidget))
	UImage* HeadImg;
	UPROPERTY(meta = (BindWidget))
	UImage* ArmorImg;
	UPROPERTY(meta = (BindWidget))
	UImage* UnderImg;
	UPROPERTY(meta = (BindWidget))
	UImage* RightImg;
	UPROPERTY(meta = (BindWidget))
	UImage* LeftImg;
	UPROPERTY(meta = (BindWidget))
	UImage* RightBowImg;
	UPROPERTY(meta = (BindWidget))
	UImage* LeftArrowImg;

};
