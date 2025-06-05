// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Data/ItemDataRow.h"

#include "UI/Inven/InventorySlot.h"

#include "InventoryScroll.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryDescriptionUpdated, const FItemData&, ItemData);
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
	void MoveSelection(FIntPoint Direction);
	void InitSelectItem();

	void EquipCurrentItem();
	void UnEquipCurrentItem();

	const FItemData& GetCurItemData()& { return ActiveSlots[CurrentIndex]->GetItemData(); }
	FVector2D GetItemListLocation();

private:
	void SetSort(EItemCategory Type);

	void InitCategory();

	//눈물을 머금고 각각바인딩...
	UFUNCTION()
	void OnWeaponCheckChanged(bool bIsChecked)		{ SelectCategory(EItemCategory::IT_Weapon, bIsChecked); }
	UFUNCTION()
	void OnArrowCheckChanged(bool bIsChecked)		{ SelectCategory(EItemCategory::IT_Arrow, bIsChecked); }
	UFUNCTION()
	void OnShieldCheckChanged(bool bIsChecked)		{ SelectCategory(EItemCategory::IT_Shield, bIsChecked); }
	UFUNCTION()
	void OnArmorCheckChanged(bool bIsChecked)		{ SelectCategory(EItemCategory::IT_Armor, bIsChecked); }
	UFUNCTION()
	void OnMaterialCheckChanged(bool bIsChecked)	{ SelectCategory(EItemCategory::IT_Material, bIsChecked); }
	UFUNCTION()
	void OnFoodCheckChanged(bool bIsChecked)		{ SelectCategory(EItemCategory::IT_Food, bIsChecked); }

	void SelectCategory(EItemCategory type, bool bIsChecked);


public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryDescriptionUpdated OnInventoryDescriptionUpdated;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlot> SlotWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* ItemWrapBox;

	UPROPERTY()
	TArray<UInventorySlot*> ActiveSlots;

	UPROPERTY()
	TArray<UInventorySlot*> PooledSlots;

	int32 CurrentIndex = 0;

public:
	UPROPERTY(meta = (BindWidget))
	UCheckBox* WeaponCheck;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* ArrowCheck;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* ShieldCheck;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* ArmorCheck;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* MaterialCheck;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* FoodCheck;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ArrowText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ArmorText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaterialText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FoodText;

private:
	UPROPERTY()
	TMap<EItemCategory, UCheckBox*> MapCategory;
	UPROPERTY()
	TMap<EItemCategory, UTextBlock*> MapCategoryText;

	EItemCategory CurrentCategory = EItemCategory::IT_Weapon;
};
