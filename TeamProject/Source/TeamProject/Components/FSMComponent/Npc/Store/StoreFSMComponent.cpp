#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	// Npc Can't Move 

	// ��ȭ ����Ǿ��� ��
	/*if (bool bBye = DeleteTalkUI()) // ��ȭ ui ���� 
	{
		ChangeState(ENpcState::Idle)
	}*/
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	
}
