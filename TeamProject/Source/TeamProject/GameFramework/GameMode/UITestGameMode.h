// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UITestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AUITestGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;



protected:
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;
};
