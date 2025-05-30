// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"

#include "UI/Quest/QuestDescription.h"
#include "UI/Quest/QuestSlot.h"

#include "Quest.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestHighlightChanged, int32, HighlightedIndex);
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuest : public UBaseUI
{
	GENERATED_BODY()
	
public:
	virtual void OnCreated() override;
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

private:
	void InitUI();
	void InitializePool(int32 PreloadCount);

	void BindDelegates();
	void RemoveDelegates();

public:
	void SetRupeeUI();

public:
	UFUNCTION()
	void RefreshQuestList(const TArray<FQuestDataRow>& QuestList);
	void RefreshQuestDescription();

public:
	void MoveSelection(FIntPoint Direction);
	void SelectInit();


public:
	FQuestDataRow GetItemDataAtIndex(int32 Index) const;

public: //바인딩을 위해 퍼블릭선언
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnCancel();

//public:
//	UPROPERTY(BlueprintAssignable)
//	FOnQuestHighlightChanged OnQuestHighlightChanged;

	int32 CurrentIndex = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UQuestSlot> SlotWidgetClass;

private:
	UPROPERTY()
	TArray<UQuestSlot*> ActiveSlots;
	UPROPERTY()
	TArray<UQuestSlot*> PooledSlots;

	UPROPERTY(meta = (BindWidget))
	UQuestDescription* BP_QuestDescription;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ItemVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UImage* HpEmptyFull;
	UPROPERTY(meta = (BindWidget))
	UImage* HpEmpty075;
	UPROPERTY(meta = (BindWidget))
	UImage* HpEmptyHalf;
	UPROPERTY(meta = (BindWidget))
	UImage* HpEmpty025;
	UPROPERTY(meta = (BindWidget))
	UImage* HpEmpty;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextCoin;

};
