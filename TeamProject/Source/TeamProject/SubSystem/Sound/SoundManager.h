// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"

#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	USoundManager();

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySoundByID(ESoundType SoundType) const;

private:
	UPROPERTY()
	UDataTable* SoundDataTable;

};
