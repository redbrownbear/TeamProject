// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Npc/Furiko/Furiko.h"
#include "Components/FSM/Npc/Furiko/FurikoFSMComponent.h"

AFuriko::AFuriko()
{		
	// FSM 
	FurikoFSMComponent = CreateDefaultSubobject<UFurikoFSMComponent>(TEXT("FurikoFSMComponent"));

	NpcFSMComponent = nullptr;
}

UFurikoFSMComponent* AFuriko::GetFSMComponent() const
{
	return FurikoFSMComponent;
}
