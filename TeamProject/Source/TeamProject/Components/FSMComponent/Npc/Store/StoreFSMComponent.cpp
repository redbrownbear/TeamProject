#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"
#include "UI/NpcDialogue/NPCDialogue.h"

#include "Actors/Npc/Npc.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
	
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Store);
	}

	if (Controller->GetConversationManager()->GetStateChanged())
	{
		ChangeState(ENpcState::Idle);
	}
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	// ���� ���� ��
	ChangeState(ENpcState::Talk); 
	
	// ���� �����ѵڿ� Idle�� ������ ��� �ٷ� Idle ���·� ���� ��
}
