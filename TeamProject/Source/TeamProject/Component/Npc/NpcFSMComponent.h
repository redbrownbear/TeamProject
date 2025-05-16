#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/FSM.h"
#include "NpcFSMComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJECT_API UNpcFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNpcFSMComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	ENpcState       eCurrentState;

public:
	ENpcState GetNPCState() const { return eCurrentState; }
	void SetNPCState(ENpcState NewState) { eCurrentState = NewState; }

private:
	void HandleState(float DeltaTime);

protected:
	void ChangeState(ENpcState NewState);

	virtual void UpdateIdle(float DeltaTime);
	virtual void UpdatePatrol(float DeltaTime);
	virtual void UpdateTalk(float DeltaTime);

};
