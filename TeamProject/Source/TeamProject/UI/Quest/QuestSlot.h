// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/QuestDataRow.h"

#include "QuestSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuestSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetQuestData(const FQuestDataRow& InQuestData);
	void SetSelected(bool bIsSelected);

public:
	const FQuestDataRow& GetQuestData() const { return QuestData; }

private:
	FQuestDataRow QuestData;

private:
	UPROPERTY(meta = (BindWidget))
	UImage* BGHighlight;
	UPROPERTY(meta = (BindWidget))
	UImage* BGNone;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextQuestName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextQuestPlace;

};
