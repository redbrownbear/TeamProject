#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/NpcDialogue/NPCDialogue.h"
#include "SubSystem/UI/QuestDialogueManager.h"


UConversationManagerComponent::UConversationManagerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;
	
	LockCharacters(Npc, Player);	

	EQuestCharacter QuestNpc = CurrentNpc->GetNpc();
	ShowTalkUI(QuestNpc);
}

void UConversationManagerComponent::EndConversation(ANpc* Npc, APlayerCharacter* Player)
{
	// Delete Talk UI

	UnlockCharacters(Npc, Player);
}

void UConversationManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		QuestDialogueManager = GameInstance->GetSubsystem<UQuestDialogueManager>();
		if (!QuestDialogueManager)
		{
			UE_LOG(LogTemp, Error, TEXT("QuestDialogueManager is NULL!"));
		}
	}
}

void UConversationManagerComponent::PlayTalkAnimations()
{
	// NPC �� ��Ÿ��
	if (UAnimInstance* NpcAnim = CurrentNpc->GetBodyMesh()->GetAnimInstance())
	{
		NpcAnim->Montage_Play(NpcTalkMontage);
		//NpcAnim->Montage_Play(NpcIdleMontage);
	}

	// Player �� ��Ÿ�� // �ʿ� ���� ����?
	if (UAnimInstance* PlayerAnim = CurrentPlayer->GetMesh()->GetAnimInstance())
	{
		PlayerAnim->Montage_Play(PlayerTalkMontage); 
		//PlayerAnim->Montage_Play(PlayerIdleMontage);
	}
}

void UConversationManagerComponent::ShowTalkUI(EQuestCharacter QuestNpc)
{
	// Create Talk UI
	if (QuestDialogueManager)
	{
		//@MODIFY: QuestDialogueManager->ShowDialogue(QuestNpc);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("QuestDialogueManager is not initialized."));
	}

	PlayTalkAnimations(); 
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
		// �̵��� ����
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_None); // �̵� �Ұ� (Jump, �ȱ� �� ��� ����)
		}

		// ȸ���� ���� ����� �ٲ�
		Player->bUseControllerRotationYaw = false;
	}
}

void UConversationManagerComponent::UnlockCharacters(ANpc* Npc, APlayerCharacter* Player)
{
	// Can Move
	if (Player)
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking); // �̵� ���� ���� ����
		}
	}

}

