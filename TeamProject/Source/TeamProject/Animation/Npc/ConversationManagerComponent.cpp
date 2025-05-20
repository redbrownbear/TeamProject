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
		// 1. NPC�� Player ���θ� �������� �ٶ󺸰�
		FVector NpcLocation = CurrentNpc->GetActorLocation();
		FVector PlayerLocation = CurrentPlayer->GetActorLocation();
		//NpcLocation.Z = PlayerLocation.Z; // ���� ȸ���� �ϵ��� ����

		CurrentPlayer->bUseControllerRotationYaw = false;
		CurrentPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;

		// 2. ���� ���� ���
		FVector PlayerToNpc = PlayerLocation - NpcLocation;
		PlayerToNpc.Normalize();

		// 3. ȸ�� ����
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
	//// NPC �� ��Ÿ��
	//if (UAnimInstance* NpcAnim = CurrentNpc->GetMesh()->GetAnimInstance())
	//{
	//	NpcAnim->Montage_Play(NpcTalkMontage);
	//}

	//// Player �� ��Ÿ��
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
		MoveComp->Deactivate(); // �̵� ��Ȱ��ȭ

	}

	if (Player)
	{
		// �Է� ����
		Player->DisableInput(nullptr);

		// �̵� �ӵ� �� ȸ�� ���� 
		Player->GetCharacterMovement()->DisableMovement(); // ���� ����
		Player->bUseControllerRotationYaw = false;
	}
}

void UConversationManagerComponent::UnlockCharacters(ANpc* Npc, APlayerCharacter* Player)
{
	// Can Move
}

