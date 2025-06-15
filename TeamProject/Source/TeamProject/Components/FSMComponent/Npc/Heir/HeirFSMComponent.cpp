#include "Components/FSMComponent/Npc/Heir/HeirFSMComponent.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

UHeirFSMComponent::UHeirFSMComponent()
{
}

void UHeirFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UHeirFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Heir);
	}

	if (Owner->GetConversationManager()->GetEndTalked())
	{
		EDialogType DType = Owner->GetCurrentDialogueType();
		if (EDialogType::None == DType)
		{
			ChangeState(ENpcState::Idle);
		}
	}
}
