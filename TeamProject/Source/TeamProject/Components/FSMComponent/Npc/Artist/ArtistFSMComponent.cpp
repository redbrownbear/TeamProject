// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Npc/Artist/ArtistFSMComponent.h"
#include "Components/SplineComponent.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

#include "Actors/StrollPath/StrollPath.h"
#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

UArtistFSMComponent::UArtistFSMComponent()
{
	eCurrentState = ENpcState::Walk;
}

void UArtistFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UArtistFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Scientist);
	}

	if (Owner->GetConversationManager()->GetEndTalked())
	{
		if (EDialogType::None == Owner->GetCurrentDialogueType())
		{
			ChangeState(ENpcState::Walk);
			Owner->SetIsHide(false);
		}
	}
}

void UArtistFSMComponent::UpdateWalk(float DeltaTime)
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