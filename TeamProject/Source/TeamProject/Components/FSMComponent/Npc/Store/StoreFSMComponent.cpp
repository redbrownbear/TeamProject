#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"
#include "UI/NpcDialogue/NPCDialogue.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	/*if (!Dialogue->GetDialogueState())
	{
		ChangeState(ENpcState::Idle);
	}*/
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	// ���� ���� ��
	ChangeState(ENpcState::Talk); 
	
	// ���� �����ѵڿ� Idle�� ������ ��� �ٷ� Idle ���·� ���� ��
}
