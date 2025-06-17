// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "ArtistFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UArtistFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()

public:
	UArtistFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
};
