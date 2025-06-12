// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ShopDataRow.h"

#include "ShopManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopUpdated, const TArray<FShopDataRow>&, ShopList);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRupeeChanged);

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShopManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UShopManager();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadShopData(UDataTable* DataTable);

	TArray<FShopDataRow> GetShopData(EQuestCharacter QuestChar) const;

public:
	void ShowUI(EQuestCharacter QuestChar, bool IsBuy);

	void UpdateShopData(EQuestCharacter QuestChar, const FShopDataRow UpdateShopRow);
	void UpdateItem(const TArray<FShopDataRow>& ShopList);

public:
	UFUNCTION()
	bool CheckSoldout();

	UFUNCTION()
	bool CanIBuyIt();

	UFUNCTION()
	void AddItemInventory();

	UFUNCTION()
	void SubtractItemInventory();

	UFUNCTION()
	void AddPlayerRupee();

	UFUNCTION()
	void SubtractPlayerRupee();

public:
	void SetSelectedItem(const FItemData& InShopData);
	FItemData GetSelectedItem() const { return SelectedShopItem.ItemData; }

private:
	void SetIsBuy(bool IsBuy) { bIsBuy = IsBuy; }
public:
	bool IsBuy() { return bIsBuy; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnShopUpdated OnShopUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnRupeeChanged OnRupeeChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* ShopDataTable;

	TMap<EQuestCharacter, TArray<FShopDataRow>> CurrentShopRowMap;

	bool bIsBuy = false;

	FShopDataRow SelectedShopItem;

	UPROPERTY()
	TArray<FShopDataRow> UpdatedShopData;

	UPROPERTY(meta = (BindWidget))
	class UShopScroll* BP_ShopScroll;
};
