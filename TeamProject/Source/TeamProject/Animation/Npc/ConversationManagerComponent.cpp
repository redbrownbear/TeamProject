#include "Animation/Npc/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Npc/Npc.h"

UConversationManagerComponent::UConversationManagerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;

	LockCharacters();
	PlayTalkAnimations();
	ShowTalkUI();
}

void UConversationManagerComponent::EndConversation()
{
	// Delete Talk UI

	UnlockCharacters();
}

void UConversationManagerComponent::BeginPlay()
{
}

void UConversationManagerComponent::PlayTalkAnimations()
{
	//// NPC ℃ 根鸥林
	//if (UAnimInstance* NpcAnim = CurrentNpc->GetMesh()->GetAnimInstance())
	//{
	//	NpcAnim->Montage_Play(NpcTalkMontage);
	//}

	//// Player ℃ 根鸥林
	//if (UAnimInstance* PlayerAnim = CurrentPlayer->GetMesh()->GetAnimInstance())
	//{
	//	PlayerAnim->Montage_Play(PlayerTalkMontage);
	//}
}

void UConversationManagerComponent::ShowTalkUI()
{
	// Create Talk UI
}

void UConversationManagerComponent::LockCharacters()
{
	if (CurrentNpc) { CurrentNpc->DisableMovement(); }
	if (CurrentPlayer) { CurrentPlayer->DisableInput(nullptr); }
}

void UConversationManagerComponent::UnlockCharacters()
{
	// Can Move
}

