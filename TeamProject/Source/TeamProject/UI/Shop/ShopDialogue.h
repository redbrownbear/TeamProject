// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Data/NPCDialogueTableRow.h"

#include "ShopDialogue.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShopDialogue : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void InitUI();
	void SetBuy();
	void SetSell();

public: //바인딩을 위해 퍼블릭선언
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnCancel();

public:
	void RefreshDialogue(const FNPCDialogueTableRow& QuestData);

private:
	void UpdateTyping();
	void OnNextButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* DialogueText;

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

	UPROPERTY(meta = (BindWidget))
	UOverlay* ActionLay;

private:
	UPROPERTY()
	FNPCDialogueTableRow DialogueDataRow;
	EQuestCharacter CurQuestChar;
	int32 NextDialogueID;

	//한글자씩 보이는 기능을 위한 변수
	FString FullText;
	int32 CurrentCharIndex = 0;
	FTimerHandle TypingTimerHandle;
	bool bIsTyping = false;

	UPROPERTY()
	TObjectPtr<ANpc> Npc = nullptr;
};
