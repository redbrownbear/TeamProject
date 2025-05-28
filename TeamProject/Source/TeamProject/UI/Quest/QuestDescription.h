// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Data/QuestDataRow.h"

#include "QuestDescription.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuestDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshUI(const FQuestDataRow& QuestData);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionTitle;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextNPC;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPlace;
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TextDescriptionBox;

	
};
