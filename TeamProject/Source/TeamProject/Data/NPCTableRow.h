// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FNPCTableRow : public FTableRowBase
{
	GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

	//NPC���� ��ȭ ��ũ��Ʈ �����ϸ� �ǾƷ��� ���ּ���
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/TeamProject.DT_NPCDialogue"))
	FDataTableRowHandle DialogueRowHandle;

};
