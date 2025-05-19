// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"
#include "Components/MultiLineEditableTextBox.h"

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

private:
	void InitUI();
	void CloseDialogue();

private:
	UFUNCTION()
	void RefreshDialogue(const FNPCDialogueTableRow& QuestData);

private:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TextBox;
	
};
