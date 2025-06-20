// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Npc/Zelda/ZeldaFSMComponent.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

UZeldaFSMComponent::UZeldaFSMComponent()
{

	// Quest Clear
	
}

void UZeldaFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UZeldaFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Zelda);
	}

	if (Owner->GetConversationManager()->GetEndTalked())
	{
		EDialogType DType = Owner->GetCurrentDialogueType();
		if (EDialogType::Quest == DType || EDialogType::None == DType)
		{
			ChangeState(ENpcState::Idle);
			Owner->GetFSMComponent()->ClearQuest(EQuestCharacter::Zelda);
		}

	}
}