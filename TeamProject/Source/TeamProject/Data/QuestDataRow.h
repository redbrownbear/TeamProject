// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NpcCharacterTableRow.h"

#include "QuestDataRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FQuestDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EQuestCharacter QuestCharacter = EQuestCharacter::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString QuestNPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString QuestPlace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector NPCWorldLocation;
};
