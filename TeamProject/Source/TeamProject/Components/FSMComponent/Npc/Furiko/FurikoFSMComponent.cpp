#include "FurikoFSMComponent.h"
#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

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

	// 플레이어 충돌 회피
	FVector MoveDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector AvoidedLocation = StrollPath->GetSplinePointLocation(CurrentStrollIndex);
	bool bAvoidingPlayer = false;

	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		float DistanceToPlayer = FVector::Dist(CurrentLocation, PlayerLocation);

		if (DistanceToPlayer < 300.0f) // 충돌 거리 임계값
		{
			FVector ToPlayer = (PlayerLocation - CurrentLocation).GetSafeNormal();
			float Dot = FVector::DotProduct(MoveDirection, ToPlayer);

			if (Dot > 0.7f) // 이동 방향과 유사할 때 충돌 예측
			{
				FVector AvoidDirection = FVector::CrossProduct(ToPlayer, FVector::UpVector);
				AvoidDirection.Normalize();
				AvoidedLocation += AvoidDirection * 600.0f; // 회피 거리
				bAvoidingPlayer = true;
			}
		}
	}

	if (bAvoidingPlayer)
	{
		MoveToLocation(AvoidedLocation);
	}
	else
	{
		MoveToLocation(TargetLocation);
	}

	// 도착 체크
	const bool bIsNear = FVector::PointsAreNear(CurrentLocation, TargetLocation, 255.f);
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

	if (Owner->GetConversationManager()->GetEndTalked())
	{
		/*bool IsConfirmed = Owner->GetIsConfirmed();
		bool IsFound = Owner->GetIsHide();
		if (!IsConfirmed || IsFound)
		{
			ChangeState(ENpcState::Run);
			Owner->SetIsHide(false);
		}*/

		if (EDialogType::None == Owner->GetCurrentDialogueType())
		{
			ChangeState(ENpcState::Run);
			Owner->SetIsHide(false);

			// Quest Clear
			bool bClearQuest = Owner->GetClearQuest();
			bool bDoQuest = Owner->GetDoQuest();
			if (!bClearQuest && bDoQuest)
			{
				Owner->GetFSMComponent()->ClearQuest(EQuestCharacter::Furiko);
				Owner->SetClearQuest(true);

				Owner->SetDoQuest(false);
			}			
		}
		else if(EDialogType::Quest == Owner->GetCurrentDialogueType())
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
