// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NpcCharacterTableRow.h"

#include "NPCDialogueTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FNPCDialogueTableRow : public FTableRowBase
{
	GENERATED_BODY();
	 
	UPROPERTY(EditAnywhere)
	EQuestCharacter QuestCharacter = EQuestCharacter::None;

	UPROPERTY(EditAnywhere)
	int32 CurrentDialogueID;

	UPROPERTY(EditAnywhere)
	int32 NextDialogueID;

	UPROPERTY(EditAnywhere)
	FString DialogueString;

	UPROPERTY(EditAnywhere)
	bool bIsEndConversation;

};