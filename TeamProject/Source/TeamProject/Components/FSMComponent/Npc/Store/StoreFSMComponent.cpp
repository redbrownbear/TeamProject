#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"
#include "Actors/Controller/Npc/Store/StoreController.h"

#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

#include "UI/NpcDialogue/NPCDialogue.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
	
	// Player ÃÄ´Ùº¸±â
	if (Owner && Player)
	{
		LookAtPlayer(Player);
	}
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (Player)
	{
		//¼öÁ¤ÇÊ¿ä
		Owner->SetNpc(EQuestCharacter::None);
	}

	if (Controller->GetConversationManager()->GetEndTalked())
	{
		ChangeState(ENpcState::Idle);
	}
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½
	ChangeState(ENpcState::Talk); 
	
	// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ÑµÚ¿ï¿½ Idleï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ ï¿½Ù·ï¿½ Idle ï¿½ï¿½ï¿½Â·ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½
}

void UStoreFSMComponent::LookAtPlayer(AActor* PlayerActor)
{
	if (!PlayerActor) return;

	FVector NpcLocation = Owner->GetActorLocation();
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	// Z°ª ¹«½ÃÇÏ°í ¼öÆò ¹æÇâ¸¸ °è»ê
	FVector Direction = (PlayerLocation - NpcLocation);
	Direction.Z = 0.f;

	if (Direction.IsNearlyZero()) return;

	FRotator LookRotation = Direction.Rotation();
	FRotator CurrentRot = Owner->GetActorRotation();
	FRotator TargetRot = LookRotation;
	float Speed = 5.0f;

	FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), Speed);
	Owner->SetActorRotation(SmoothRot);
}

bool UStoreFSMComponent::CanSeePlayer() const
{
	if (!Owner || !Player) return false;

	AStoreController* KoroguCon = Cast<AStoreController>(Owner->GetController());
	if (!KoroguCon) return false;

	return KoroguCon->LineOfSightTo(Player);
}

