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

	static ConstructorHelpers::FObjectFinder<UDataTable> DialogueTableAsset(TEXT("/Game/Data/NPC/DT_NPCDialogue.DT_NPCDialogue"));
	if (DialogueTableAsset.Succeeded())
	{
		NpcDialogueTable = DialogueTableAsset.Object;
	}

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;

	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UQuestDialogueManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
	check(QuestManager);

	if (UIManager && QuestManager)
	{
		if (QuestManager->IsConversation())
			return;

		UIManager->ShowUI(UNPCDialogue::StaticClass());
		
		EDialogType DialogType = Npc->GetCurrentDialogueType();
		EQuestCharacter QuestCharacter = Npc->GetData()->QuestCharacter;
		int32 DialogueID = 0;

		/*if ((QuestCharacter == EQuestCharacter::Furiko && (DialogType == EDialogType::None || DialogType == EDialogType::Quest)) ||
			(QuestCharacter == EQuestCharacter::Korok && (DialogType == EDialogType::None || DialogType == EDialogType::Shop)))
		{
			DialogueID = GetDialogueID(NpcDialogueTable, QuestCharacter, DialogType);
		}*/

		DialogueID = GetDialogueID(NpcDialogueTable, QuestCharacter, DialogType);

		QuestManager->ShowDialogue(CurrentNpc->GetData()->QuestCharacter, DialogueID);
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

int32 UConversationManagerComponent::GetDialogueID(UDataTable* DialogueTable, EQuestCharacter Character, EDialogType DialogType)
{
	if (!DialogueTable) return -1;

	const TArray<FName> RowNames = DialogueTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		const FNPCDialogueTableRow* Row = DialogueTable->FindRow<FNPCDialogueTableRow>(RowName, TEXT("Dialogue Lookup"));
		if (!Row) continue;

		if (Row->QuestCharacter == Character && Row->DialogType == DialogType)
		{
			return Row->CurrentDialogueID;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No matching dialogue found for Character: %d, DialogType: %d"), (uint8)Character, (uint8)DialogType);
	return -1;
}


void UConversationManagerComponent::PlayTalkAnimations()
{
	if (UAnimInstance* NpcAnim = CurrentNpc->GetBodyMesh()->GetAnimInstance())
	{
		NpcAnim->Montage_Play(NpcTalkMontage);
		//NpcAnim->Montage_Play(NpcIdleMontage);
	}

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
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_None); 
		}

		Player->bUseControllerRotationYaw = false;
	}
}

void UConversationManagerComponent::UnlockCharacters(ANpc* Npc, APlayerCharacter* Player)
{
	if (Player)
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking); 
		}
	}
}

