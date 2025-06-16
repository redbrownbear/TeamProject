// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"

#include "InputActionValue.h"

#include "UI/Loading/LoadingWithPlayerInfo.h"

#include "TitleWidget.generated.h"

class ATitlePlayerController;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartClicked();
public:
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnConfirm();

private:
	void OnFadeOutStep();
	void PlayFadeOutAndStart();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* Continue;
	UPROPERTY(meta = (BindWidget))
	UButton* NewGame;
	UPROPERTY(meta = (BindWidget))
	UButton* Exit;

	UPROPERTY(meta = (BindWidget))
	UImage* ImgContinue;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgNewGame;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgExit;

public:
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> LoadingWidgetSoftClass;

private:
	int CurrentStep = 0;
	const int StepCount = 30;
	const float StepTime = 0.033f;
	FTimerHandle FadeHandle;

	bool bHasInitialized = false;
};
