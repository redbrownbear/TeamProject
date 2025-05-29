// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Data/ItemDataRow.h"

#include "InventoryManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const FItemData&, ItemData);
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//테스트코드
	void AddItem(FItemData Itemdata);

public:
	const TArray<FItemData>& GetAllItemData() const { return Items; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

private:
	TArray<FItemData> Items;

};
