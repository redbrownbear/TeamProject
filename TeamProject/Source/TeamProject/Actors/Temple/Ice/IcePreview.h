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

    void AnimateRise(float Value);

    void UpdateLocation(const FVector& WorldLocation);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY()
    TObjectPtr<UTimelineComponent> PreviewTimeline;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> RiseCurve;

private:
    bool bIsAnimating = false;

protected:
    UMaterialInterface* MaterialInterface;
    UMaterialInstanceDynamic* DynamicMaterialInstance;

};
