#include "NpcFSMComponent.h"

UNpcFSMComponent::UNpcFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNpcFSMComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNpcFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UNpcFSMComponent::HandleState(float DeltaTime)
{
	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
	case ENpcState::Patrol:
		UpdatePatrol(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	default:
		break;
	}
}

void UNpcFSMComponent::ChangeState(ENpcState NewState)
{
	if (eCurrentState == NewState) { return; }
	switch (NewState)
	{
	case ENpcState::Idle:
		break;
	case ENpcState::Patrol:
		break;
	case ENpcState::Talk:
		break;	
	default:
		break;
	}
	eCurrentState = NewState;
}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
}

void UNpcFSMComponent::UpdatePatrol(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateTalk(float DeltaTime)
{
}
