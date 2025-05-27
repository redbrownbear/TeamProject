#include "FurikoFSMComponent.h"
#include "Actors/Npc/Npc.h"

#include "Actors/StrollPath/StrollPath.h"
#include "Components/SplineComponent.h"

#include "UI/NpcDialogue/NPCDialogue.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

UFurikoFSMComponent::UFurikoFSMComponent()
{
	eCurrentState = ENpcState::Run;
}

void UFurikoFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UFurikoFSMComponent::UpdateRun(float DeltaTime)
{	
	Super::UpdateRun(DeltaTime);		

	// 스플라인 경로
	AStrollPath* StrollPath = Owner->GetStrollPath();
	if (!StrollPath)
	{
		UE_LOG(LogTemp, Error, TEXT("UFurikoFSMComponent::UpdateStroll // No StrollPath"));
		return;
	}

	FVector TargetLocation = StrollPath->GetSplinePointLocation(CurrentStrollIndex);
	FVector CurrentLocation = Owner->GetActorLocation();

	// 이동
	MoveToLocation(TargetLocation);

	// 도착 체크
	const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), TargetLocation, 255.f);
	if (bIsNear)
	{
		++CurrentStrollIndex;
		if (CurrentStrollIndex >= StrollPath->GetSplineMaxIndex())
		{
			CurrentStrollIndex = 0;
		}
	}
}

void UFurikoFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);
	
	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Furiko);
	}

	if (Controller->GetConversationManager()->GetEndTalked())
	{
		bool IsConfirmed = Owner->GetIsConfirmed();
		bool IsFound = Owner->GetIsHide();
		if (!IsConfirmed || IsFound)
		{
			ChangeState(ENpcState::Run);
			Owner->SetIsHide(false);
		}
		else
		{
			ChangeState(ENpcState::Hide);
			Owner->SetIsConfirmed(false);
		}		
	}
}

void UFurikoFSMComponent::UpdateHide(float DeltaTime)
{
	Super::UpdateHide(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Furiko);
	}
	 
}
