// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Npc/Npc.h"
#include "Controller/Npc/Furiko/FurikoController.h"
#include "Furiko.generated.h"

class UFurikoFSMComponent;

UCLASS()
class TEAMPROJECT_API AFuriko : public ANpc
{
	GENERATED_BODY()

public:
	AFuriko();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	TObjectPtr<UFurikoFSMComponent> FurikoFSMComponent;

public:
	UFurikoFSMComponent* GetFSMComponent() const;
};
