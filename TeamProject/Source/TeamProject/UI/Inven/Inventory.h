// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnhancedInputComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/ItemDataRow.h"

#include "UI/Base/BaseUI.h"
#include "UI/Inven/InventoryScroll.h"
#include "UI/Inven/InventoryEquip.h"
#include "UI/Inven/InventorySelectSet.h"
#include "Actors/Item/WorldWeapon.h"

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
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

private:
	void InitUI();
	void BindDelegates();
	void RemoveDelegate();

	void SetRupeeUI();

public: //바인딩을 위해 퍼블릭선언
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	void OnConfirm(const FInputActionValue& InputActionValue);
	void OnCancel(const FInputActionValue& InputActionValue);
	void OnCreateItemInWorld(const FInputActionValue& InputActionValue);

	void OnCreateItemTest(const FInputActionValue& InputActionValue);

private:
	UFUNCTION()
	void RefreshInventory(const FItemData& ItemData);
	UFUNCTION()
	void RefreshAllInventory(const TArray<FItemData>& ItemDataList);
	UFUNCTION()
	void RefreshDescription(const FItemData& ItemData);
	UFUNCTION()
	void RefreshEquip(const TArray<FItemData>& ItemDataMap);


private:
	UPROPERTY(meta = (BindWidget))
	UInventoryScroll* BP_InvenScroll;

	UPROPERTY(meta = (BindWidget))
	UInventoryEquip* BP_InvenEquip;

	UPROPERTY(meta = (BindWidget))
	UInventorySelectSet* BP_InvenSelectSet;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AWorldWeapon> WorldItemActorClass;

};