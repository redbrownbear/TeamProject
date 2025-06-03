// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/ItemDataRow.h"

#include "PopupGetItem.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPopupGetItem : public UBaseUI
{
	GENERATED_BODY()

public:
	virtual void OnCreated() override;
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

private:
	void InitUI();

public:
	void ShowData(FItemData ItemData);

	UFUNCTION()
	void OnCancel();

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ImgItemIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* ImgItemCategory;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemDescription;
};
