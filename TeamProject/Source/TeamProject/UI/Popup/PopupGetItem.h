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

public: //바인딩을 위해 퍼블릭선언
	UFUNCTION()
	virtual void OnNavigate(const FInputActionValue& Value) override {}
	UFUNCTION()
	virtual void OnConfirm(const FInputActionValue& Value) override {}
	UFUNCTION()
	virtual void OnCancel(const FInputActionValue& Value) override;

private:
	void InitUI();

public:
	void ShowData(FItemData ItemData);

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
