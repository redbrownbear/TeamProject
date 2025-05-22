// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/NPCDialogueTableRow.h"

#include "QuestDialogueManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisPlayDialogueUpdated, const FNPCDialogueTableRow&, DialogueRows);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TEAMPROJECT_API UQuestDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadDialogueData(UDataTable* DataTable);

	TArray<const FNPCDialogueTableRow*> GetDialogueData(EQuestCharacter QusetChar) const;

public:
	void ShowDialogue(EQuestCharacter QusetChar, int32 DialogueID);

public:
	//대화를 위한 델리게이트 구독
	UFUNCTION()
	void HandleNextDialogueRequested(EQuestCharacter QuestChar, int32 CurrentID);

public:
	void SetConversation(bool isTalk) { bIsNowConversation = isTalk; }
	bool IsConversation() { return bIsNowConversation; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnDisPlayDialogueUpdated OnDialogueUpdated;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* QuestDataTable;

private:
	TMap<EQuestCharacter, TArray<const FNPCDialogueTableRow*>> QuestRowMap;

private:
	bool bIsNowConversation = false;

};
