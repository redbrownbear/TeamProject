// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Data/ItemDataRow.h"

#include "InventoryManager.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UInventoryManager();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadItemData(UDataTable* DataTable);


public:
	const TArray<FItemData>& GetAllItemData() const { return Items; }


	private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* ItemDataTable;

private:
	TArray<FItemData> Items;

};
