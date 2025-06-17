// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "MusicianFSMComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMusicianFSMComponent : public UNpcFSMComponent
{
	GENERATED_BODY()

public:
	UMusicianFSMComponent();

protected:
	virtual void UpdateIdle(float DeltaTime) override;
	
};
