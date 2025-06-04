// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Data/ItemDataRow.h"

#include "InventorySelectSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipUpdated, const FItemData&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUseUpdated, const FItemData&, ItemData);

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventorySelectSet : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	void BindDelegates();
	void RemoveDelegate();

public:
	void ShowButton(bool bIsShow, FItemData InItemData);

	UFUNCTION()
	void UseItme();
	UFUNCTION()
	void Cancel();


private:
	void ChangeEquip();
	void UseItem();

public:
	UPROPERTY(BlueprintAssignable)
	FOnItemEquipUpdated OnItemEquipUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnItemUseUpdated OnItemUseUpdated;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonUse;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonCancel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextUse;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextCancel;


	FItemData ItemData;
};
