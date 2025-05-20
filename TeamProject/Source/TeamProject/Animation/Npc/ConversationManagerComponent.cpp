#include "Animation/Npc/ConversationManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Npc/Npc.h"
#include "GameFramework/FloatingPawnMovement.h"

UConversationManagerComponent::UConversationManagerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConversationManagerComponent::StartConversation(ANpc* Npc, APlayerCharacter* Player)
{
	CurrentNpc = Npc;
	CurrentPlayer = Player;

	if (CurrentPlayer && CurrentNpc)
	{
		// 1. NPC와 Player 서로를 수평으로 바라보게
		FVector NpcLocation = CurrentNpc->GetActorLocation();
		FVector PlayerLocation = CurrentPlayer->GetActorLocation();
		//NpcLocation.Z = PlayerLocation.Z; // 수평 회전만 하도록 보정

		CurrentPlayer->bUseControllerRotationYaw = false;
		CurrentPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;

		// 2. 방향 벡터 계산
		FVector PlayerToNpc = PlayerLocation - NpcLocation;
		PlayerToNpc.Normalize();

		// 3. 회전 수행
		//SmoothRotateActorToDirection(CurrentNpc, PlayerLocation, GetWorld()->GetDeltaSeconds());	
		SmoothRotateActorToDirection(CurrentPlayer, NpcLocation, GetWorld()->GetDeltaSeconds());
		RotateActorToDirection(CurrentNpc, PlayerLocation);
	}

	LockCharacters(CurrentNpc, CurrentPlayer);
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
	// Create Talk UI
}

void UConversationManagerComponent::LockCharacters(ANpc* Npc, APlayerCharacter* Player)
{	
	if (!Npc)
	{
		UE_LOG(LogTemp, Error, TEXT("LockCharacters: Npc is null!"));
		return;
	}

	if (UFloatingPawnMovement* MoveComp = Cast<UFloatingPawnMovement>(Npc->GetMovementComponent()))
	{
		MoveComp->Deactivate(); // 이동 비활성화

	}

	if (Player)
	{
		// 입력 차단
		Player->DisableInput(nullptr);

		// 이동 속도 및 회전 제한 
		Player->GetCharacterMovement()->DisableMovement(); // 완전 정지
		Player->bUseControllerRotationYaw = false;
	}
}

void UConversationManagerComponent::UnlockCharacters(ANpc* Npc, APlayerCharacter* Player)
{
	// Can Move
}

