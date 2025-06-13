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

public:
	UFUNCTION()
	virtual void OnNavigate(const FInputActionValue& Value) override;
	UFUNCTION()
	virtual void OnConfirm(const FInputActionValue& Value) override;
	UFUNCTION()
	virtual void OnCancel(const FInputActionValue& Value) override;

	void OnCategoryLeft(const FInputActionValue& InputActionValue);
	void OnCategoryRight(const FInputActionValue& InputActionValue);

public:
	void SetDialogueData(EQuestCharacter InQuestChar, int32 InDialogueID);

public:
	void SetIsBuyScroll(bool IsBuy) { bIsBuyScroll = IsBuy; }
	bool IsBuyScroll() { return bIsBuyScroll; }

public:
	void OnBuy();
	void OnSell();

private:
	void InitUI();

	UFUNCTION()
	void SetRupeeUI();

private:
	void StartCoinEffect(int32 FinalCoinValue);
	void UpdateCoinEffect();

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

private:
	EQuestCharacter QuestChar;
	int32 DialogueID;

private:
	bool bIsBuyScroll;
	FTimerHandle CoinUpdateTimerHandle;
	int32 CurrentDisplayedCoin;
	int32 TargetCoin;
	int32 CoinStep;
	float UpdateInterval;
};
