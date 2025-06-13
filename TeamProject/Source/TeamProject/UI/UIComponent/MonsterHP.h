// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Progressbar.h"

#include "MonsterHP.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMonsterHP : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

public:
	void ShowUI(float CurHp, float MaxHp);
	void HideUI();

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBarHp;

	FTimerHandle HideTimerHandle;
};
