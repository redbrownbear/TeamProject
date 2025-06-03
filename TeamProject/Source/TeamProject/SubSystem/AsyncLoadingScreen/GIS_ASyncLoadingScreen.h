// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Loading/LoadingWithPlayerInfo.h"

#include "GIS_ASyncLoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UGIS_ASyncLoadingScreen : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen(TSubclassOf<UUserWidget> WidgetClass);
	UFUNCTION(BlueprintCallable)
	void OpenLevelWithLoadingScreenTitle(TSubclassOf<UUserWidget> WidgetClass, const TSoftObjectPtr<UWorld> Level);
	UFUNCTION(BlueprintCallable)
	void OpenLevelWithLoadingScreen(const TSoftObjectPtr<UWorld> Level);

	UFUNCTION(BlueprintCallable)
	void OpenLevelWithLoadingScreenNonAsynchronous(TSubclassOf<UUserWidget> WidgetClass, const TSoftObjectPtr<UWorld> Level);


public:
	void SetLoadingUI(TSubclassOf<ULoadingWithPlayerInfo> WidgetClass) { LoadingWidgetClass = WidgetClass; }

private:
	TSubclassOf<ULoadingWithPlayerInfo> LoadingWidgetClass;
};
