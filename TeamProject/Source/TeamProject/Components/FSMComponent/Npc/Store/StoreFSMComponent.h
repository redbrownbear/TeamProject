// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "StoreFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UStoreFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateTalk(float DeltaTime) override;
	virtual void UpdateSell(float DeltaTime);
};
