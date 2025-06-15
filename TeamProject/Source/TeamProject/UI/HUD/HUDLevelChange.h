// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "HUDLevelChange.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UHUDLevelChange : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void ShowLevelChangeUI(FString LevelName);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextLevel;

	float FadeTime = 0.f;
	bool bFadingIn = true;

	bool bOepnLevel = false;
};
