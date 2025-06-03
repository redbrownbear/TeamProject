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
    float MaxSpeed = 300.f;            // 1�ʿ� �ö� �ӵ� (cm/sec)

    UPROPERTY(EditAnywhere, Category = "Cryonis")
    float MaxHeight = 300.0f;         // �� �ö� �Ÿ�

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
