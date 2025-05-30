// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"

#include "InputActionValue.h"

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

	void OnHovored();
	void OffHovored();

	UFUNCTION()
	void OnStartClicked();
public:
	UFUNCTION()
	void OnNavigate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnConfirm();

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
};
