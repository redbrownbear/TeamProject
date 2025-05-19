#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	// Npc Can't Move 

	// 대화 종료되었을 때
	/*if (bool bBye = DeleteTalkUI()) // 대화 ui 삭제 
	{
		ChangeState(ENpcState::Idle)
	}*/
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	
}
