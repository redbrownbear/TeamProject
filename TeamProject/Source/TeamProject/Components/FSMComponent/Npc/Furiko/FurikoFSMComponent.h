#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/NPC/NpcFSMComponent.h"
#include "Actors/Controller/Npc/Furiko/FurikoController.h"
#include "FurikoFSMComponent.generated.h"

UCLASS()
class TEAMPROJECT_API UFurikoFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void HandleState(float DeltaTime);

	void ChangeState(ENpcState NewState);

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateStroll(float DeltaTime) override;
	virtual void UpdateTalk(float DeltaTime) override;
	virtual void UpdateHide(float DeltaTime);
	virtual void UpdatePlay(float DeltaTime); // // 술래잡기: 시간 되면 추가

private:
	UPROPERTY()
	AFurikoController* FurikoController = nullptr;
};
