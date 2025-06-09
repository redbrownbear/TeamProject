// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/ItemDataRow.h"

#include "QuickItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuickItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void LoadIconTexture();

public:
	void InitFirstSlot(eEquipParts Category);

private:
	UTexture2D* GetCategoryIcon(eEquipParts Category) const;

public:
	void SetItemData(const FItemData& Item);
	const FItemData& GetItemData() const { return ItemData; }

public:
	void SetSelected(bool bIsSelected);
	void SetEquiped(bool bIsEquiped);

	void SetEquipDate();
private:
	FItemData ItemData;

	UTexture2D* WeaponCategory;
	UTexture2D* ShieldCategory;
	UTexture2D* BowCategory;
	UTexture2D* ArrowCategory;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
	UPROPERTY(meta = (BindWidget))
	UImage* HighlightBg;
	UPROPERTY(meta = (BindWidget))
	UImage* EquipedBg;
	UPROPERTY(meta = (BindWidget))
	UImage* CategoryIconImg;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Amount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Value;
};
