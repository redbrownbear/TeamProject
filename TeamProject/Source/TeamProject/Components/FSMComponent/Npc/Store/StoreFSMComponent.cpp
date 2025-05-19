#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	// Npc Can't Move 
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	
}
