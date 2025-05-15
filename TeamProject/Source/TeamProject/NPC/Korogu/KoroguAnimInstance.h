#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KoroguFSMComponent.h"
#include "KoroguAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UKoroguAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKoroguAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	UKoroguFSMComponent* FSMComponent = nullptr;

	ENpcState eCurrentState;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false; // 술래잡기: 상황 봐서 여유되면 추가
	UPROPERTY(BlueprintReadOnly)
	bool bIsTalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHide = false;
};
