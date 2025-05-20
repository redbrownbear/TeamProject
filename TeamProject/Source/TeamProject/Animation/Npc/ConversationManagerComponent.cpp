#include "Animation/Npc/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Npc/Npc.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "UI/NpcDialogue/NPCDialogue.h"

UConversationManagerComponent::UConversationManagerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;

	LockCharacters(Npc, Player);
	PlayTalkAnimations();
	ShowTalkUI();
}

void UConversationManagerComponent::EndConversation(ANpc* Npc, APlayerCharacter* Player)
{
	// Delete Talk UI

	//UnlockCharacters(Npc, Player);
}

void UConversationManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UConversationManagerComponent::PlayTalkAnimations()
{
	//// NPC → 몽타주
	//if (UAnimInstance* NpcAnim = CurrentNpc->GetMesh()->GetAnimInstance())
	//{
	//	NpcAnim->Montage_Play(NpcTalkMontage);
	//}

	//// Player → 몽타주
	//if (UAnimInstance* PlayerAnim = CurrentPlayer->GetMesh()->GetAnimInstance())
	//{
	//	PlayerAnim->Montage_Play(PlayerTalkMontage);
	//}
}

void UConversationManagerComponent::ShowTalkUI()
{
	//// Create Talk UI
	//Dialogue->OnCreated();

	//PlayTalkAnimations();
}

void UConversationManagerComponent::LockCharacters(ANpc* Npc, APlayerCharacter* Player)
{	
	if (!Npc)
	{
		UE_LOG(LogTemp, Error, TEXT("LockCharacters: Npc is null!"));
		return;
	}

	if (Player)
	{
		// 이동만 제한
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->DisableMovement(); // 이동 불가 (Jump, 걷기 등 모두 막힘)
		}

		// 회전도 수동 제어로 바꿈
		Player->bUseControllerRotationYaw = false;
	}
}

void UConversationManagerComponent::UnlockCharacters(ANpc* Npc, APlayerCharacter* Player)
{
	// Can Move
}

