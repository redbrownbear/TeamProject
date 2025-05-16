// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SubSystem/UI/InventoryManager.h"
#include "UI/Inven/InventorySlot.h"

#include "Blueprint/UserWidget.h"
#include "InventoryScroll.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventoryScroll : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateSlots(const TArray<FItemData>& NewItemList);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> SlotWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY()
	TArray<UInventorySlot*> SlotPool;


};
