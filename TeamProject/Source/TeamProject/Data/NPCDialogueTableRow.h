// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCTableRow.h"

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
	TArray<FString> DialogueArr;

};