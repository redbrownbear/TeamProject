#include "KoroguFSMComponent.h"

// Sets default values for this component's properties
UKoroguFSMComponent::UKoroguFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKoroguFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UKoroguFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UKoroguFSMComponent::HandleState(float DeltaTime)
{
	switch (eCurrentState)
	{
	case EKoroguState::Idle:
		UpdateIdle(DeltaTime);
	case EKoroguState::Patrol:
		UpdatePatrol(DeltaTime);
		break;
	case EKoroguState::Hide:
		UpdateHide(DeltaTime);
	default:
		break;
	}
}

void UKoroguFSMComponent::ChangeState(EKoroguState NewState)
{
	if (eCurrentState == NewState) { return; }
	switch (NewState)
	{
	case EKoroguState::Idle:
		break;
	case EKoroguState::Patrol:
		break;
	case EKoroguState::Hide:		
	default:
		break;
	}
	eCurrentState = NewState;
}

void UKoroguFSMComponent::UpdateIdle(float DeltaTime)
{

}

void UKoroguFSMComponent::UpdatePatrol(float DeltaTime)
{
}

void UKoroguFSMComponent::UpdateHide(float DeltaTime)
{
}
