#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/NpcDialogue/NPCDialogue.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PC_InGame.h"
#include "UI/HUD/MainHUD.h"


UConversationManagerComponent::UConversationManagerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;
	
	//LockCharacters(Npc, Player);

	APC_InGame* PC = Cast<APC_InGame>(CurrentPlayer->GetController());
	check(PC);

	AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD());
	check(HUD)

	if (PC && HUD)
		HUD->ShowInteractWidget(false);
			
	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
	check(QuestManager);

	if (UIManager && QuestManager)
	{
		if (QuestManager->IsConversation())
			return;

		UIManager->ShowUI(UNPCDialogue::StaticClass());	

		bool IsQuest = Npc->GetDoQuest();
		if (!DialogueDataRow.bIsEndConversation && IsQuest)
		{
			CurrentNpc->GetData()->QuestCharacter = EQuestCharacter::Furiko_Found;
			QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, 101);
		}
		else 
		{
			CurrentNpc->GetData()->QuestCharacter = EQuestCharacter::Furiko;
			QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, 0);
		}			
	}

}

void UConversationManagerComponent::EndConversation()
{	
	//UnlockCharacters(CurrentNpc, CurrentPlayer);	
	
	bEndTalk = true;
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
	if (Player)
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking); // �̵� ���� ���� ����
		}
	}
}

