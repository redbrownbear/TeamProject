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
	virtual void ShowUI() override;
	virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

private:
	void InitUI();
	void BindDelegates();
	void RemoveDelegates();

public: //Î∞îÏù∏?©ÏùÑ ?ÑÌï¥ ?ºÎ∏îÎ¶?Ñ†??
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnNextDialogue(const FInputActionValue& InputActionValue);

private:
	UFUNCTION()
	void RefreshDialogue(const FNPCDialogueTableRow& QuestData);

	void UpdateTyping();
	void OnNextButtonClicked();


private:
	UPROPERTY()
	FNPCDialogueTableRow DialogueDataRow;
	EQuestCharacter CurQuestChar;
	int32 NextDialogueID;

	//«—±€¿⁄æø ∫∏¿Ã¥¬ ±‚¥…¿ª ¿ß«— ∫Øºˆ
	FString FullText;
	int32 CurrentCharIndex = 0;
	FTimerHandle TypingTimerHandle;
	bool bIsTyping = false;

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