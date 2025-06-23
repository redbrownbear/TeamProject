// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"
#include "UI/QuickSlot/QuickItemSlot.h"

#include "Components/WrapBox.h"
#include "Components/Scrollbox.h"
#include "Data/ItemDataRow.h"

#include "QuickSlotMain.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuickSlotMain : public UBaseUI
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
	virtual void OnConfirm(const FInputActionValue& Value) override {}
	UFUNCTION()
	virtual void OnCancel(const FInputActionValue& Value) override;

private:
	void InitUI();
	void BindDelegates();
	void RemoveDelegate();

	void InitializePool(int32 PreloadCount);

	UFUNCTION()
	void RefreshItems(const TArray<FItemData>& ItemDataList);

public:
	void RefreshFirstSlot(eEquipParts Parts);

private:
	void MoveSelection(FIntPoint Direction);
	void EquipCrrentItem();

private:
	UPROPERTY()
	TArray<UQuickItemSlot*> ActiveSlots;
	UPROPERTY()
	TArray<UQuickItemSlot*> PooledSlots;

	int32 CurrentIndex = 0;
	eEquipParts CurrentPart = eEquipParts::NONE;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UQuickItemSlot> SlotWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UQuickItemSlot* InitSlot;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* ItemWrapBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;
	
};
