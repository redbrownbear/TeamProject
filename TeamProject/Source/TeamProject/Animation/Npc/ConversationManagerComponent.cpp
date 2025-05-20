#include "Animation/Npc/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Npc/Npc.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/NpcDialogue/NPCDialogue.h"
#include "SubSystem/UI/UIManager.h"


UConversationManagerComponent::UConversationManagerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;
	
	LockCharacters(Npc, Player);	
	ShowTalkUI();
}

void UConversationManagerComponent::EndConversation(ANpc* Npc, APlayerCharacter* Player)
{
	// Delete Talk UI

	UnlockCharacters(Npc, Player);
}

void UConversationManagerComponent::BeginPlay()
{
	Super::BeginPlay();
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

void UConversationManagerComponent::ShowTalkUI()
{
	// Create Talk UI
	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	if (UIManager)
	{
		// Dialogue UI
		FString Path = TEXT("/Game/Blueprint/UI/NpcDialogue/BP_NpcDialogue.BP_NpcDialogue_C");
		TSubclassOf<UNPCDialogue> NPCDialogueClass = LoadClass<UBaseUI>(nullptr, *Path);

		UNPCDialogue* DialogueUI = UIManager->CreateUI<UNPCDialogue>(GetWorld(), NPCDialogueClass);
		if (!DialogueUI)
		{
			UE_LOG(LogTemp, Error, TEXT("StartConversation: DialogueUI ���� ���� �Ǵ� �̹� ����"));
		}
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

