// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "DogFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UDogFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()
	
public:
	UDogFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateWalk(float DeltaTime) override;
	virtual void UpdateRun(float DeltaTime) override;
	virtual void UpdateNotify(float DeltaTime);
	virtual void UpdateFind(float DeltaTime);
};
