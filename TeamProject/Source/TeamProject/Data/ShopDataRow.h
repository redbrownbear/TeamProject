// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Data/NPCDialogueTableRow.h"
#include "Data/ItemDataRow.h"

#include "ShopDataRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FShopDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EQuestCharacter QuestCharacter = EQuestCharacter::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 InitialItemCount;

};
