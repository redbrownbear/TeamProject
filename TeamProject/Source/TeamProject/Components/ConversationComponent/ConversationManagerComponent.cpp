#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/NpcDialogue/NPCDialogue.h"
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

	if (PC)
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->ShowInteractWidget(false);
		}

		PC->ShowDialogueUI();
	}	

}

void UConversationManagerComponent::EndConversation()
{	
	//UnlockCharacters(CurrentNpc, CurrentPlayer);	
	
	bStateChange = true;
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
	// NPC → 몽타주
	if (UAnimInstance* NpcAnim = CurrentNpc->GetBodyMesh()->GetAnimInstance())
	{
		NpcAnim->Montage_Play(NpcTalkMontage);
		//NpcAnim->Montage_Play(NpcIdleMontage);
	}

	// Player → 몽타주 // 필요 없을 수도?
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
		// 이동만 제한
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_None); // 이동 불가 (Jump, 걷기 등 모두 막힘)
		}

		// 회전도 수동 제어로 바꿈
		Player->bUseControllerRotationYaw = false;
	}
}

void UConversationManagerComponent::UnlockCharacters(ANpc* Npc, APlayerCharacter* Player)
{
	if (Player)
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking); // 이동 가능 상태 복원
		}
	}
}

