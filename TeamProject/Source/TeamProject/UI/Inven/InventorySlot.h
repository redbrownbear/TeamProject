// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Data/ItemDataRow.h"

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
	void SetItemData(const FItemData& Item);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Amount;
};
