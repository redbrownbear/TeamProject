#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlapTempleBall);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTorchesLit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAssasinBossDead);

class ATorchStand;

UCLASS()
class TEAMPROJECT_API UEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// -------- Temple Switches --------
	UFUNCTION()
	void NotifyOverlapTempleBall();

	// -------- Torches --------
	UFUNCTION()
	void RegisterTorch(ATorchStand* Torch);
	UFUNCTION()
	void NotifyTorchLit(ATorchStand* Torch);

	// -------- MonsterDead --------
	UFUNCTION()
	void AssasinBossDead();
	
public:
	// -------- Temple Switches --------
	UPROPERTY(BlueprintAssignable)
	FOnOverlapTempleBall OnOverlapTempleBall;

	// -------- Torches --------
	UPROPERTY(BlueprintAssignable)
	FOnAllTorchesLit OnAllTorchesLit;

	// -------- MonsterDead --------
	UPROPERTY(BlueprintAssignable)
	FOnAssasinBossDead OnAssasinBossDead;

private:
	// -------- Temple Switches --------
	TArray<TObjectPtr<ATorchStand>> TorchList;
	TSet<TObjectPtr<ATorchStand>> LitTorchSet;
};
