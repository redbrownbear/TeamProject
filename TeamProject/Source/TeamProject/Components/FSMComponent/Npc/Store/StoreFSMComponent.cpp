#include "Components/FSMComponent/Npc/Store/StoreFSMComponent.h"
#include "Actors/Controller/Npc/Store/StoreController.h"

#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

#include "UI/NpcDialogue/NPCDialogue.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

void UStoreFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
	
	// Player 쳐다보기
	if (Owner && Player)
	{
		LookAtPlayer(Player);
	}
}

void UStoreFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (!Player)
	{
		//수정필요
		Owner->SetNpc(EQuestCharacter::None);
	}

	if (Owner->GetConversationManager()->GetEndTalked())
	{
		ChangeState(ENpcState::Idle);
	}
}

void UStoreFSMComponent::UpdateSell(float DeltaTime)
{
	ChangeState(ENpcState::Talk); 
	
}

void UStoreFSMComponent::LookAtPlayer(AActor* PlayerActor)
{
	if (!PlayerActor) return;

	FVector NpcLocation = Owner->GetActorLocation();
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	// Z값 무시하고 수평 방향만 계산
	FVector Direction = (PlayerLocation - NpcLocation);
	Direction.Normalize();

	if (Direction.IsNearlyZero()) return;

	FRotator LookRotation = Direction.Rotation();
	FRotator CurrentRot = Owner->GetActorRotation();
	FRotator TargetRot = LookRotation;
	float Speed = 5.0f;

	FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), Speed);
	Owner->SetActorRotation(SmoothRot);
}

//bool UStoreFSMComponent::CanSeePlayer() const
//{
//	if (!Owner || !Player) return false;
//
//	AStoreController* KoroguCon = Cast<AStoreController>(Owner->GetController());
//	if (!KoroguCon) return false;
//
//	return KoroguCon->LineOfSightTo(Player);
//}

