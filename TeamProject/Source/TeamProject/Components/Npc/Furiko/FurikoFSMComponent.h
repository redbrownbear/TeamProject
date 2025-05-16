#pragma once

#include "CoreMinimal.h"
#include "Components/NPC/NpcFSMComponent.h"
#include "Controller/Npc/Furiko/FurikoController.h"
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
	virtual void UpdatePlay(float DeltaTime); // �������: ��Ȳ ���� �����Ǹ� �߰�

private:
	UPROPERTY()
	AFurikoController* FurikoController = nullptr;
};
