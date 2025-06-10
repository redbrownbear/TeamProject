// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/TextBlock.h"

#include "UI/Shop/ShopDescription.h"
#include "UI/Shop/ShopDialogue.h"
#include "UI/Shop/ShopScroll.h"
#include "UI/Shop/ShopSellScroll.h"

#include "Shop.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShop : public UBaseUI
{
	GENERATED_BODY()
	
public:
	virtual void OnCreated() override;
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;


private:
	void InitUI();
	void SetRupeeUI();

private:
	void BindDelegates();
	void RemoveDelegates();

private:
	void SetShopOpen();

	void SetItemBuy();

	UFUNCTION()
	void RefreshAllInventory(const TArray<FItemData>& ItemDataList);

public: 
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnNextDialogue(const FInputActionValue& InputActionValue);

private:
	UFUNCTION()
	void RefreshDialogue(const FNPCDialogueTableRow& QuestData);
	UFUNCTION()
	void RefreshDescription(int32 CurrentIdx);
	UFUNCTION()
	void RefreshDescriptionSellItem(const FItemData& ItemData);

	UFUNCTION()
	void RefreshShopList(const TArray<FShopDataRow>& ShopList);

private:
	UPROPERTY(meta = (BindWidget))
	UShopDescription* BP_ShopDescription;

	UPROPERTY(meta = (BindWidget))
	UShopDialogue* BP_ShopDialogue;

	UPROPERTY(meta = (BindWidget))
	UShopScroll* BP_ShopScroll;

	UPROPERTY(meta = (BindWidget))
	UShopSellScroll* BP_ShopSellScroll;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;

};
