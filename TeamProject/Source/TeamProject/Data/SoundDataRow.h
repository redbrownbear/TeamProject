// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundDataRow.generated.h"


UENUM(BlueprintType)
enum class ESoundType : uint8
{
    ESound_Confirm,
	ESound_Cancel,
    ESound_GetItem,
    ESound_LostItem,
    ESound_FindFuriko,

	ESound_MAX = 254,
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAMPROJECT_API FSoundData : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SoundID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Sound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Volume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Pitch = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bLoop = false;

};
