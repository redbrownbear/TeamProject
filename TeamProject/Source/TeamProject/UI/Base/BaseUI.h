// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseUI : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void OnCreated();	//����� �ʱ�ȭ
	virtual void CloseUI();

	void PauseAllPausableActors(bool bPause);
};
