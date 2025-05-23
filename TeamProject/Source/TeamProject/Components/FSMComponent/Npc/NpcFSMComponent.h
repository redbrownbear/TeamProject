#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Utils.h"
#include "NpcFSMComponent.generated.h"

class ANpc;
class APlayerCharacter;
class ANpcController;
class UNPCDialogue;
class AHidePoint;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJECT_API UNpcFSMComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ANpc;
	friend class APC_InGame;

public:
	UNpcFSMComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetOwner(ANpc* InOwner) { Owner = InOwner; }
	void SetPlayer(APlayerCharacter* InPlayer) { Player = InPlayer; }

	ANpcController* GetController() const { return Controller; }
	UNPCDialogue* GetNpcDialogue() const { return Dialogue; }

protected:
	UPROPERTY()
	TObjectPtr<ANpc> Owner = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player = nullptr;

	UPROPERTY()
	TObjectPtr<ANpcController> Controller = nullptr;

	// 대화 종료 시 사용할 변수
	UPROPERTY()
	TObjectPtr<UNPCDialogue> Dialogue = nullptr;


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
	virtual void UpdateSit(float DeltaTime);
	virtual void UpdateStand(float DeltaTime);
	virtual void UpdateWalk(float DeltaTime);
	virtual void UpdateRun(float DeltaTime);
	virtual void UpdateTalk(float DeltaTime);
	virtual void UpdateHide(float DeltaTime);
	virtual void UpdateSell(float DeltaTime);
	virtual void UpdateEnd(float DeltaTime);

protected:
	void MoveToLocation(const FVector& InLocation);

	void HideFuriko();
	void SetHideLocation(FVector InLocation);

public:
	UPROPERTY(EditAnywhere, Category = "Teleport")
	TArray<FVector> TeleportLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HidePoint")
	TArray<AHidePoint*> HidePoints;

private:
	float AcceptanceRadius = 50.0f;

};
