// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UMainHUDWidget;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
private:
    virtual void BeginPlay() override;

public:
    void ShowInteractWidget(bool bIsShow);

protected:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMainHUDWidget> MainWidgetClass;

    UPROPERTY()
    UMainHUDWidget* MainWidgetInstance;

};
