// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "ZeldaFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UZeldaFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()
	
public:
	UZeldaFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateTalk(float DeltaTime) override;
};
