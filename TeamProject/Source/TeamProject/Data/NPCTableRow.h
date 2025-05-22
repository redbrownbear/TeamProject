// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCTableRow.generated.h"

UENUM()
enum class EQuestCharacter
{
	None,

	Furiko,

	Store,

	End,
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FNPCTableRow : public FTableRowBase
{
	GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

	//NPC와의 대화 스크립트 가능하면 맨아래에 놔주세요
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/TeamProject.DT_NPCDialogue"))
	FDataTableRowHandle DialogueRowHandle;

};
