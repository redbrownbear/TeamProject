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

	APC_InGame* PC = Cast<APC_InGame>(CurrentPlayer->GetController());
	check(PC);

	AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD());
	check(HUD)

		if (PC && HUD)
			HUD->ShowInteractWidget(false);

	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UShopManager* ShopManager = GetWorld()->GetGameInstance()->GetSubsystem<UShopManager>();
	check(ShopManager);

	UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
	check(QuestManager);

	if (UIManager && QuestManager && ShopManager)
	{
		if (QuestManager->IsConversation())
			return;

		EDialogType DialogType = Npc->GetData()->DialogType;
		if (DialogType == EDialogType::Shop)
		{
			//임시
			FShopDataRow datarow;

			UIManager->ShowUI(UShop::StaticClass());
			ShopManager->UpdateShopData(CurrentNpc->GetData()->QuestCharacter, datarow);
			QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Store));

			bool IsShopping = Npc->GetShopping();
			bool IsBuying = Npc->GetBuy();

			
			//if (!DialogueDataRow.bIsEndConversation && !IsShopping && !IsBuying)
			//{
			//	QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, 200);
			//}
			//else if (!DialogueDataRow.bIsEndConversation && IsShopping && IsBuying)
			//{
			//	// 구매 시
			//	QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, 1000);
			//}
			//else if (!DialogueDataRow.bIsEndConversation && IsShopping && !IsBuying)
			//{
			//	// 구매 안 할 시
			//	QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, 1100);
			//}

		}				
		else
		{

			UIManager->ShowUI(UNPCDialogue::StaticClass());
			bool IsQuest = Npc->GetDoQuest();
			if (!DialogueDataRow.bIsEndConversation && IsQuest)
			{
				QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Furiko_Found));
			}
			else
			{
				QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, static_cast<int32>(EQuestCharDialogue::Furiko));
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

