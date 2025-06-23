#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlapTempleBall);
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

	// -------- MonsterDead --------
	UFUNCTION()
	void AssasinBossDead();

public:
	// -------- Temple Switches --------
	UPROPERTY(BlueprintAssignable)
	FOnOverlapTempleBall OnOverlapTempleBall;

	// -------- MonsterDead --------
	UPROPERTY(BlueprintAssignable)
	FOnAssasinBossDead OnAssasinBossDead;

};
