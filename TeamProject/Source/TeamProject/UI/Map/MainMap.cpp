// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Map/MainMap.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PC_InGame.h"


void UMainMap::OnCreated()
{
    InitUI();
}

void UMainMap::ShowUI()
{
    Super::ShowUI();

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {

        PC_InGame->ChangeInputContext(EInputContext::IC_Dialogue);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
    }
}

void UMainMap::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    Super::HideUI(UQuickSlotMain::StaticClass());
}

void UMainMap::InitUI()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->BindDialogueInput();
    }
}

void UMainMap::SetMapData(const TArray<FMapTileInfo>& InTiles)
{
    MapTiles = InTiles;

    if (MapTiles.Num() == 0) return;

    MinWorld = FVector2D(MapTiles[0].WorldLocation.X, MapTiles[0].WorldLocation.Y);
    MaxWorld = FVector2D(MapTiles[0].WorldLocation.X, MapTiles[0].WorldLocation.Y);

    for (const auto& Tile : MapTiles)
    {
        MinWorld.X = FMath::Min(MinWorld.X, Tile.WorldLocation.X);
        MinWorld.Y = FMath::Min(MinWorld.Y, Tile.WorldLocation.Y);
        MaxWorld.X = FMath::Max(MaxWorld.X, Tile.WorldLocation.X);
        MaxWorld.Y = FMath::Max(MaxWorld.Y, Tile.WorldLocation.Y);
    }
}

int32 UMainMap::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (MapTiles.Num() == 0) return LayerId;

    FVector2D Size = AllottedGeometry.GetLocalSize();

    for (const FMapTileInfo& Tile : MapTiles)
    {
        FVector2D NormalizedPos = {
            (Tile.WorldLocation.X - MinWorld.X) / (MaxWorld.X - MinWorld.X),
            (Tile.WorldLocation.Y - MinWorld.Y) / (MaxWorld.Y - MinWorld.Y)
        };

        FVector2D DrawPos = NormalizedPos * Size;

        FLinearColor Color = Tile.bIsWalkable ? FLinearColor::White : FLinearColor::Gray;
        Color *= FMath::Clamp(Tile.Height / 300.0f, 0.2f, 1.0f); // 높이에 따른 명암

        FVector2D SizeBox = FVector2D(2.0f, 2.0f); // 타일 크기

        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(DrawPos, SizeBox),
            FCoreStyle::Get().GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            Color
        );
    }

    return LayerId + 1;
}
