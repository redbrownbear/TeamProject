// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DataTable/ItemData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

#include "InventoryManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, FItemData, ItemData);
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void AddItem(FItemData Itemdata);


public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

private:
	TArray<FItemData> Items;

};
