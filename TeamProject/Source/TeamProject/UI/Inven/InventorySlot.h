// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SubSystem/UI/InventoryManager.h"

#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void InitSlot(const FItemData& Item);


private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;
};
