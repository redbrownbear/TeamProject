// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

#include "HUDMiniMap.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UHUDMiniMap : public UUserWidget
{
	GENERATED_BODY()
	
private:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    FVector2D ConvertWorldToMapPosition(const FVector& WorldLocation, const FVector2D& CurrentPanelSize) const;

private:
    static const FVector2D MINI_MAP_MIN_WORLD;
    static const FVector2D MINI_MAP_MAX_WORLD;

private:

    mutable FVector PlayerWorldPos;
    mutable bool bHasUpdatedPlayerPos = false;

private:
    UPROPERTY(meta = (BindWidget))
    UImage* ImgPlayer;
    UPROPERTY(meta = (BindWidget))
    UImage* ImgMinimap;
    //UPROPERTY(meta = (BindWidget))
    //UImage* ImageQuest;

};
