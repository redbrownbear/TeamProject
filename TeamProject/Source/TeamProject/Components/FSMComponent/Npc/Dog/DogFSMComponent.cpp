// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Npc/Dog/DogFSMComponent.h"

#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

UDogFSMComponent::UDogFSMComponent()
{
}

void UDogFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UDogFSMComponent::UpdateWalk(float DeltaTime)
{
	Super::UpdateWalk(DeltaTime);

	if (Player)
	{
		// Player 위치 받아서 따라다니기
	}
	// 센서로 보물 감지 시 ChangeState
}

void UDogFSMComponent::UpdateRun(float DeltaTime)
{
	Super::UpdateRun(DeltaTime);
}

void UDogFSMComponent::UpdateNotify(float DeltaTime)
{
	if (eCurrentState != ENpcState::Notify)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Notify'"));
		return;
	}
}

void UDogFSMComponent::UpdateFind(float DeltaTime)
{
	if (eCurrentState != ENpcState::Find)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Find'"));
		return;
	}
}
