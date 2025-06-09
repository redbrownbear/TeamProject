// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Map/MainMap.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PC_InGame.h"
#include "Components/CanvasPanelSlot.h"

const FVector2D UMainMap::MAP_MIN_WORLD = FVector2D(2850.f, -34510.f);
const FVector2D UMainMap::MAP_MAX_WORLD = FVector2D(36350.f, -17850.f);

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
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
    }
}

void UMainMap::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    Super::HideUI(UMainMap::StaticClass());
}

void UMainMap::InitUI()
{
    ImageQuest->SetVisibility(ESlateVisibility::Collapsed);
}

FVector2D UMainMap::ConvertWorldToMapPosition(const FVector& WorldLocation, const FVector2D& CurrentPanelSize) const
{
    float NormalizedX = (WorldLocation.X - MAP_MIN_WORLD.X) / (MAP_MAX_WORLD.X - MAP_MIN_WORLD.X);
    float NormalizedY = (WorldLocation.Y - MAP_MIN_WORLD.Y) / (MAP_MAX_WORLD.Y - MAP_MIN_WORLD.Y);

    FVector2D MapPos;
    MapPos.X = FMath::Clamp(NormalizedX, 0.f, 1.f) * CurrentPanelSize.X;
    MapPos.Y = FMath::Clamp(NormalizedY, 0.f, 1.f) * CurrentPanelSize.Y;

    return MapPos;
}

void UMainMap::SetPlayerData()
{
    
}

void UMainMap::OnCancel()
{
    HideUI(UMainMap::StaticClass());
}

int32 UMainMap::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
    const APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (!PlayerPawn)
        return LayerId + 1;
    
    if (PlayerWorldPos == GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation())
        return LayerId + 1;

    FVector2D PanelSize = ImageMap->GetCachedGeometry().GetLocalSize();

    UCanvasPanelSlot* MapPanelSlot = Cast<UCanvasPanelSlot>(SizeBoxMap->Slot);
    check(MapPanelSlot);

    UCanvasPanelSlot* PlayerPanelSlot = Cast<UCanvasPanelSlot>(ImagePlayer->Slot);
    check(PlayerPanelSlot);

    PlayerWorldPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    FVector2D MapPos = ConvertWorldToMapPosition(PlayerWorldPos, PanelSize);

    PlayerPanelSlot->SetPosition(MapPos);

    float Yaw = PlayerPawn->GetActorRotation().Yaw;

    FWidgetTransform Transform;
    Transform.Angle = Yaw + 90.0f;

    ImagePlayer->SetRenderTransform(Transform);
    
    return LayerId + 1;
}
