// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/HUDMiniMap.h"

const FVector2D UHUDMiniMap::MINI_MAP_MIN_WORLD = FVector2D(2850.f, -34510.f);
const FVector2D UHUDMiniMap::MINI_MAP_MAX_WORLD = FVector2D(36350.f, -17850.f);

void UHUDMiniMap::NativeConstruct()
{
    FVector2D CanvasSize = ImgPlayer->GetCachedGeometry().GetLocalSize();

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ImgPlayer->Slot);
    check(PanelSlot);

    PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f)); 
    PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));

    PanelSlot->SetPosition(CanvasSize * 0.5f);

    FString MapName = GetWorld()->GetMapName();
    if (MapName.Contains(TEXT("GameMap")))
    {
        SetVisibility(ESlateVisibility::Visible);
        UpdateCanTick();
    }
    else   
        SetVisibility(ESlateVisibility::Hidden);
       
}

void UHUDMiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    const APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
    const APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (!PlayerPawn)
        return;

    FVector CurrentWorldPos = PlayerPawn->GetActorLocation();

    if (PlayerWorldPos == CurrentWorldPos)
        return;

    PlayerWorldPos = CurrentWorldPos;

    FVector2D PanelSize = ImgMinimap->GetCachedGeometry().GetLocalSize();
    FVector2D CanvasSize = ImgPlayer->GetCachedGeometry().GetLocalSize();

    UCanvasPanelSlot* MapPanelSlot = Cast<UCanvasPanelSlot>(ImgMinimap->Slot);
    check(MapPanelSlot);

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ImgPlayer->Slot);
    check(PanelSlot);

    PanelSlot->SetPosition(FVector2d(CanvasSize.X * 0.05f, CanvasSize.X * -0.5f));

    FVector2D MapPos = ConvertWorldToMapPosition(PlayerWorldPos, PanelSize);
    FVector2D Offset = PanelSize * 0.5f - MapPos;

    MapPanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
    MapPanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));

    MapPanelSlot->SetPosition(Offset);

    float Yaw = PlayerPawn->GetActorRotation().Yaw;
    FWidgetTransform Transform;
    Transform.Angle = Yaw + 90.0f;
    ImgPlayer->SetRenderTransform(Transform);
}

FVector2D UHUDMiniMap::ConvertWorldToMapPosition(const FVector& WorldLocation, const FVector2D& CurrentPanelSize) const
{
    float NormalizedX = (WorldLocation.X - MINI_MAP_MIN_WORLD.X) / (MINI_MAP_MAX_WORLD.X - MINI_MAP_MIN_WORLD.X);
    float NormalizedY = (WorldLocation.Y - MINI_MAP_MIN_WORLD.Y) / (MINI_MAP_MAX_WORLD.Y - MINI_MAP_MIN_WORLD.Y);

    FVector2D MapPos;
    MapPos.X = FMath::Clamp(NormalizedX, 0.f, 1.f) * CurrentPanelSize.X;
    MapPos.Y = FMath::Clamp(NormalizedY, 0.f, 1.f) * CurrentPanelSize.Y;

    return MapPos;
}
