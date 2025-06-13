// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextDataRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FTextData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 TextID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Text;

};
