#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FurikoFSMComponent.h"
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
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false; // �������: ��Ȳ ���� �����Ǹ� �߰�
	UPROPERTY(BlueprintReadOnly)
	bool bIsTalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHide = false;
};
