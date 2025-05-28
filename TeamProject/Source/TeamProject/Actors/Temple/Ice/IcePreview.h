#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IcePreview.generated.h"

class UTimelineComponent;

UCLASS()
class TEAMPROJECT_API AIcePreview : public AActor
{
	GENERATED_BODY()
	
public:
    AIcePreview();

public:
    UMaterialInstanceDynamic* GetMaterialInstance() { return DynamicMaterialInstance; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

protected:
    UPROPERTY(EditAnywhere, Category = "Cryonis")
    float MaxSpeed = 300.f;            // 1초에 올라갈 속도 (cm/sec)

    UPROPERTY(EditAnywhere, Category = "Cryonis")
    float MaxHeight = 300.0f;         // 총 올라갈 거리

protected:
    UMaterialInterface* MaterialInterface;
    UMaterialInstanceDynamic* DynamicMaterialInstance;

private:
    FVector StartLocation;
    float CurrentRise = 0.f;
    bool bIsRising = false;
    bool bCanSpawn = false;

    FVector RiseDirection;
    FVector PivotLocation;
public:
    void SetRiseDirection(FVector InDirection);
    void SetPivotLocation(FVector InPosition);
    void SetCanSpawn(bool bFlag) { bCanSpawn = bFlag; }
};
