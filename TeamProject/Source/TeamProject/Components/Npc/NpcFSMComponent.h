#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/StrollPath.h"
#include "NpcFSMComponent.generated.h"

UENUM()
enum class ENpcState : uint8
{
	Idle,
	Sit,
	Stand,
	Walk,
	Stroll,
	Talk,
	Hide,
	Play, // 술래잡기: 상황 봐서 여유되면 추가
};

class ANpc;
class ANpcController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJECT_API UNpcFSMComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ANpcController;

public:
	UNpcFSMComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetOwner(ANpc* InOwner) { Owner = InOwner; }

protected:
	UPROPERTY()
	TObjectPtr<ANpc> Owner = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AStrollPath> StrollPathActor;

protected:
	ENpcState       eCurrentState;

public:
	ENpcState GetNpcState() const { return eCurrentState; }
	void SetNpcState(ENpcState NewState) { eCurrentState = NewState; }

private:
	void HandleState(float DeltaTime);

protected:
	void ChangeState(ENpcState NewState);

	virtual void UpdateIdle(float DeltaTime);
	virtual void UpdateStroll(float DeltaTime);
	virtual void UpdateTalk(float DeltaTime);

protected:
	void MoveToLocation(const FVector& InLocation);

private:
	int32 CurrentStrollIndex = 0;
	float AcceptanceRadius = 50.0f;

private:
	UPROPERTY()
	ANpcController* NpcController = nullptr;
};
