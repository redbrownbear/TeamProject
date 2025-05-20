// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Data/NPCDialogueTableRow.h"

#include "NPCDialogue.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UNPCDialogue : public UBaseUI
{
	GENERATED_BODY()

public:
	virtual void OnCreated() override;
	virtual void CloseUI() override;

private:
	void InitUI();

private:
	UFUNCTION()
	void RefreshDialogue(const FNPCDialogueTableRow& QuestData);

private:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TextBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExtraButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConfrimText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CancelText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExtraText;
	
};
