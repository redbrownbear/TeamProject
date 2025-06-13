// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

#include "HUDQuestOn.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UHUDQuestOn : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	
public:
	void ShowQuestUI(bool bIsComplete, FString QuestName);

private:
	void PlaySlideIn();
	void AnimateToOrigin();
	void PlaySlideOut();
	void AnimateOut();


private:
	UPROPERTY(meta = (BindWidget))
	USizeBox* BoxQuest;
	UPROPERTY(meta = (BindWidget))
	UImage* ImageQuestBg;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextQuestName;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	UTexture2D* QuestOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	UTexture2D* QuestCoplete;

private:
	FTimerHandle HideTimerHandle;
	FTimerHandle SlideInHandle;
	FTimerHandle SlideOutHandle;

	bool bHasInitialized = false;

	static FVector2D OriginPosition;

};
