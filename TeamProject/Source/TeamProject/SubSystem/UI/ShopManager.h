// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ShopDataRow.h"

#include "ShopManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopUpdated, const TArray<FShopDataRow>&, ShopList);

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

	//������
	void UpdateShopData(EQuestCharacter QuestChar, const FShopDataRow UpdateShopRow);

	//UI
	void UpdateItem(const TArray<FShopDataRow>& ShopList);

public:
	UFUNCTION()
	bool CheckSoldout();

	UFUNCTION()
	void AddItemInventory();

	UFUNCTION()
	void SubtractItemInventory();

	UFUNCTION()
	bool CanIBuyIt();

	UFUNCTION()
	void AddPlayerRupee();

	UFUNCTION()
	void SubtractPlayerRupee();

	UFUNCTION()
	void AddShopItem();

	UFUNCTION()
	void SubtractShopItem();

//public:
//	void SetShopDataRow(FShopDataRow InShopData) { SelectedShopItem = InShopData; }
//
//
//	// --- ShopDataRow 복사본 데이터 전달 ---
//	SelectedShopItem.ItemData = ItemData;
//	SetShopDataRow(SelectedShopItem);


private:
	void SetIsBuy(bool IsBuy) { bIsBuy = IsBuy; }
public:
	bool IsBuy() { return bIsBuy; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnShopUpdated OnShopUpdated;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* ShopDataTable;

	TMap<EQuestCharacter, TArray<FShopDataRow>> ShopRowMap;

	bool bIsBuy = false;

	FShopDataRow SelectedShopItem;

	UPROPERTY(meta = (BindWidget))
	class UShopScroll* BP_ShopScroll;
};
