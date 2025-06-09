// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

#include "Actors/Map/MapDataExtractor.h"

#include "MainMap.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMainMap : public UBaseUI
{
	GENERATED_BODY()

public:
    virtual void OnCreated() override;
    virtual void ShowUI() override;
    virtual void HideUI(TSubclassOf<UBaseUI> UIClass) override;

private:
    void InitUI();
    FVector2D ConvertWorldToMapPosition(const FVector& WorldLocation, const FVector2D& CurrentPanelSize) const;

public:
    void SetPlayerData();

    UFUNCTION()
    void OnCancel();

protected:
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
        bool bParentEnabled) const override;

private:
    static const FVector2D MAP_MIN_WORLD;
    static const FVector2D MAP_MAX_WORLD;

private:
    TArray<FMapTileInfo> MapTiles;

    mutable FVector PlayerWorldPos;
    mutable bool bHasUpdatedPlayerPos = false;

private:
    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBoxMap;
   
    UPROPERTY(meta = (BindWidget))
    UImage* ImageMap;
    UPROPERTY(meta = (BindWidget))
    UImage* ImagePlayer;
    UPROPERTY(meta = (BindWidget))
    UImage* ImageQuest;

};
