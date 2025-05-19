// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/NPCDialogueTableRow.h"

#include "QuestDialogueManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisPlayDialogueUpdated, const FNPCDialogueTableRow&, ItemData);
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuestDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	const FNPCDialogueTableRow* GetItemRow(EQuestCharacter qusetChar) const;

public:
	void ShowDialogue(EQuestCharacter QusetChar);

public:
	UPROPERTY(BlueprintAssignable)
	FOnDisPlayDialogueUpdated OnDialogueUpdated;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ItemDataTable;

private:
	TMap<EQuestCharacter, const FNPCDialogueTableRow*> ItemRowMap;

};
