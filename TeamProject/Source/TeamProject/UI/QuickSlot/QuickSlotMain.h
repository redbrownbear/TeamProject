// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"
#include "UI/QuickSlot/QuickItemSlot.h"

#include "Components/WrapBox.h"
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

private:
	void InitUI();
	void BindDelegates();
	void RemoveDelegate();

	void InitializePool(int32 PreloadCount);
	void RefreshFirstSlot(eEquipParts Parts);

	UFUNCTION()
	void RefreshItems(const TArray<FItemData>& ItemDataList);

private:
	UPROPERTY()
	TArray<UQuickItemSlot*> ActiveSlots;
	UPROPERTY()
	TArray<UQuickItemSlot*> PooledSlots;

	int32 CurrentIndex = 0;
	EItemCategory CurrentCategory = EItemCategory::IT_Weapon;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UQuickItemSlot> SlotWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UQuickItemSlot* InitSlot;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* ItemWrapBox;
	
};
