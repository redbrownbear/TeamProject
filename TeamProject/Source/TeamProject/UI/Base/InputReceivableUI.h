// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "InputReceivableUI.generated.h"


UINTERFACE(Blueprintable)
class UInputReceivableUI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAMPROJECT_API IInputReceivableUI
{
	GENERATED_BODY()

public:
	virtual void OnNavigate(const FInputActionValue& Value) = 0;
	virtual void OnConfirm(const FInputActionValue& Value) = 0;
	virtual void OnCancel(const FInputActionValue& Value) = 0;

public:
};
