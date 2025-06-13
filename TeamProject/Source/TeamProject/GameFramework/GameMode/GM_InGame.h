// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "UI/Loading/LoadingWithPlayerInfo.h"

#include "GM_InGame.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AGM_InGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_InGame();

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere, Category = "Loading")
	TSubclassOf<ULoadingWithPlayerInfo> LoadingWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Loading")
	TSubclassOf<UUserWidget> LoadingWidgetToTitleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MonsterHPWidgetClass;

};
