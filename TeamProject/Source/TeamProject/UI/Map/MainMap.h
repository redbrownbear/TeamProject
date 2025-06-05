// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseUI.h"
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

public:
    void SetMapData(const TArray<FMapTileInfo>& InTiles);

protected:
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
        bool bParentEnabled) const override;

private:
    TArray<FMapTileInfo> MapTiles;
    FVector2D MinWorld, MaxWorld;

};
