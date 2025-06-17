// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "ScientistFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UScientistFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()
	
public:
	UScientistFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
};
