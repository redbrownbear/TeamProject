#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/NpcDialogue/NPCDialogue.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
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

	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
	check(QuestManager);

	if (UIManager && QuestManager)
	{
		if (QuestManager->IsConversation())
			return;

		EDialogType DialogType = Npc->GetData()->DialogType;

		//로직 수정 해야함
		//임시 수정해놓음 -> EQuestCharDialogue::Furiko_Found 이런 하드코딩 지양하길 바라 ㅠㅠ
		UIManager->ShowUI(UNPCDialogue::StaticClass());
		bool IsQuest = Npc->GetDoQuest();
		if (!DialogueDataRow.bIsEndConversation && IsQuest)
		{
			//QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Furiko_Found));
			QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, QUESTCHARDIALOGUE_FURIKO_FIND);
		}
		else
		{
			if (DialogType == EDialogType::Shop)
			{
				//QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Store));
				QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, QUESTCHARDIALOGUE_STORE);

			}
			else
			{
				//QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Furiko));
				QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, QUESTCHARDIALOGUE_FURIKO);
			}
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

