#pragma once

#include "CoreMinimal.h"
#include "NPC/NpcFSMComponent.h"
#include "KoroguFSMComponent.generated.h"

UCLASS()
class TEAMPROJECT_API UKoroguFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()

private:
	void HandleState(float DeltaTime);

protected:
	void ChangeState(ENpcState NewState);

	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdatePatrol(float DeltaTime) override;
	virtual void UpdateTalk(float DeltaTime) override;
	virtual void UpdateHide(float DeltaTime);
	virtual void UpdatePlay(float DeltaTime); // 술래잡기: 상황 봐서 여유되면 추가
};
