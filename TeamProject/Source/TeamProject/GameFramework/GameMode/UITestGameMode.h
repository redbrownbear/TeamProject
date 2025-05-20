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


	//전부테스트용
public:
	void CreatePopup();
	void OnOpenPopup();

	void CreateInven();
	void OnOpenInven();

	void CreateItem();

	void CreateDialogueBox();
	void OnOepnDialogueBox();

	void OnpenQuest();


protected:
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;
};
