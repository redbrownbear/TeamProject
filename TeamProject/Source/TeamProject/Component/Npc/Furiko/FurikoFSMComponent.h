#pragma once

#include "CoreMinimal.h"
#include "Component/NPC/NpcFSMComponent.h"
#include "FurikoFSMComponent.generated.h"

UCLASS()
class TEAMPROJECT_API UFurikoFSMComponent : public UNpcFSMComponent
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
	virtual void UpdatePlay(float DeltaTime); // �������: ��Ȳ ���� �����Ǹ� �߰�
};
