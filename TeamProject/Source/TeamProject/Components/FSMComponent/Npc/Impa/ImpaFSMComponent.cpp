#include "Components/FSMComponent/Npc/Impa/ImpaFSMComponent.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

UImpaFSMComponent::UImpaFSMComponent()
{
}

void UImpaFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UImpaFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Impa);
	}

	if (Owner->GetConversationManager()->GetEndTalked())
	{
		EDialogType DType = Owner->GetCurrentDialogueType();
		if (EDialogType::Quest == DType || EDialogType::None == DType)
		{
			ChangeState(ENpcState::Idle);
		}

		// Quest Clear
		Owner->GetFSMComponent()->ClearQuest(EQuestCharacter::Impa);
	}
}
