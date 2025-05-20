#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"
#include "StoreAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class TEAMPROJECT_API UStoreAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UStoreAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	UStoreFSMComponent* FSMComponent = nullptr;

	ENpcState eCurrentState;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsTalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSell = false;
};
