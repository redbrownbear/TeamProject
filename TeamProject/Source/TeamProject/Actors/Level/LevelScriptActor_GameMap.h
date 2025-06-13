// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptActor_GameMap.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ALevelScriptActor_GameMap : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
