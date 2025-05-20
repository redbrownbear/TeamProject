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
	// 물건 구매 시
	ChangeState(ENpcState::Talk); 
	
	// 구매 ㄳㄳ한뒤에 Idle로 돌릴지 사고 바로 Idle 상태로 돌릴 지
}
