// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TextManager.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTextManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UTextManager();

public:
	UFUNCTION(BlueprintCallable, Category = "Text")
	FString GetTextByID(int32 TextID) const;

private:
	UPROPERTY()
	UDataTable* TextDataTable;

};
