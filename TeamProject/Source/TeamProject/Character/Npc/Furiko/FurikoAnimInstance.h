#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/Fsm/Npc/Furiko/FurikoFSMComponent.h"
#include "FurikoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UFurikoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFurikoAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	UFurikoFSMComponent* FSMComponent = nullptr;

	ENpcState eCurrentState;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSit = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStand = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false; 
	UPROPERTY(BlueprintReadOnly)
	bool bIsTalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHide = false;
};
