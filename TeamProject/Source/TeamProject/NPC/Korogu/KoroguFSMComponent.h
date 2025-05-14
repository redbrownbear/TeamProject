#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/FSM.h"
#include "KoroguFSMComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UKoroguFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKoroguFSMComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	EKoroguState       eCurrentState;
public:
	EKoroguState GetMonsterState() const { return eCurrentState; }
	void SetMonsterState(EKoroguState NewState) { eCurrentState = NewState; }

protected:
	float SuspicionGauge = 0.0f;
	float MaxSuspicionGauge = 100.0f;

private:
	void HandleState(float DeltaTime);

protected:
	void ChangeState(EKoroguState NewState);

	virtual void UpdateIdle(float DeltaTime);
	virtual void UpdatePatrol(float DeltaTime);
	virtual void UpdateHide(float DeltaTime);
};
