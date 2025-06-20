// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/FSMComponent/Npc/Artist/ArtistFSMComponent.h"
#include "ArtistAnimInstance.generated.h"


class UArtistFSMComponent;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UArtistAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UArtistAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<UArtistFSMComponent> ArtistFSMComponent = nullptr;

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
