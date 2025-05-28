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

    void StartPreview();
    void StopPreview();

public:
    UMaterialInstanceDynamic* GetMaterialInstance() { return DynamicMaterialInstance; }
    void SetMaterialInstance(UMaterialInstanceDynamic* InMaterialInstance) { DynamicMaterialInstance = InMaterialInstance; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

private:
    float RiseSpeed = 3.f;            // 1초에 올라갈 속도 (cm/sec)
    float RiseDistance = 250.f;         // 총 올라갈 거리

    FVector StartLocation;
    FVector TargetLocation;
    float CurrentRise = 0.f;
    bool bIsRising = false;
    
protected:
    UMaterialInterface* MaterialInterface;
    UMaterialInstanceDynamic* DynamicMaterialInstance;

};
