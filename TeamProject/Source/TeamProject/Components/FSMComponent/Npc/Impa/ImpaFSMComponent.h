// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "ImpaFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UImpaFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()

public:
	UImpaFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	virtual void UpdateTalk(float DeltaTime) override;

private:
	bool bIsTalking = false;
};