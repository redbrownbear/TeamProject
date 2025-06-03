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

	//µ•¿Ã≈Õ
	void UpdateShopData(EQuestCharacter QuestChar, const FShopDataRow UpdateShopRow);

	//UI
	void UpdateItem(const TArray<FShopDataRow>& ShopList);


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
};
