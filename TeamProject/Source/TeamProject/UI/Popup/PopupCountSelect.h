// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Data/ItemDataRow.h"

#include "PopupCountSelect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDropUpdated, const FItemData&, ItemData);

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPopupCountSelect : public UBaseUI
{
	GENERATED_BODY()

public:
	virtual void OnCreated() override;
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

public:
	UFUNCTION()
	virtual void OnNavigate(const FInputActionValue& Value) override;
	UFUNCTION()
	virtual void OnConfirm(const FInputActionValue& Value) override;
	UFUNCTION()
	virtual void OnCancel(const FInputActionValue& Value) override;

protected:
	UFUNCTION()
	void OnPlusClick();
	UFUNCTION()
	void OnMinusClick();
	UFUNCTION()
	void OnCancelClick();


protected:
	void BindDelegates();
	void RemoveDelegate();

private:
	void InitUI();
	void MoveSelection(FIntPoint Direction);

public:
	void SetUI(const FItemData& InItemData);

public:
	UPROPERTY(BlueprintAssignable)
	FOnItemDropUpdated OnItemDropUpdated;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextCount;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnMinus;
		
	UPROPERTY(meta = (BindWidget))
	UButton* BtnPlus;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonCancel;

	int32 MaxCount;
	int32 CurrentCount;
	FItemData ItemData;
};
