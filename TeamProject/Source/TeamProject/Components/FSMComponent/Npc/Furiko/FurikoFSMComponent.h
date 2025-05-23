#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/NPC/NpcFSMComponent.h"
#include "Actors/Controller/Npc/Furiko/FurikoController.h"
#include "FurikoFSMComponent.generated.h"

class AHidePoint;

UCLASS()
class TEAMPROJECT_API UFurikoFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()

public:
	UFurikoFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateRun(float DeltaTime) override;
	virtual void UpdateTalk(float DeltaTime) override;
	virtual void UpdateHide(float DeltaTime) override;
	//virtual void UpdatePlay(float DeltaTime); // 술래잡기: 시간 되면 추가

protected:
	void HideFuriko();
	void SetActorLocation(FVector InLocation);

public:
	UPROPERTY(EditAnywhere, Category = "Teleport")
	TArray<FVector> TeleportLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HidePoint")
	TArray<AHidePoint*> HidePoints;

private:
	int32 CurrentStrollIndex = 0;

};
