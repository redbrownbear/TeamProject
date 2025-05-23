#include "FurikoFSMComponent.h"
#include "Actors/Npc/Npc.h"

#include "Actors/StrollPath/StrollPath.h"
#include "Components/SplineComponent.h"

#include "UI/NpcDialogue/NPCDialogue.h"
#include "Components/ConversationComponent/ConversationManagerComponent.h"

#include "Actors/HidePoint/FurikoHidePoint.h" 

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

	if (Controller->GetConversationManager()->GetStateChanged())
	{
		ChangeState(ENpcState::Run);
	}
}

void UFurikoFSMComponent::UpdateHide(float DeltaTime)
{
	Super::UpdateHide(DeltaTime);

	if (Player)
	{
		Owner->SetNpc(EQuestCharacter::Furiko);
	}

	if (Controller->GetConversationManager()->GetStateChanged())
	{
		ChangeState(ENpcState::Talk);
	}
}

void UFurikoFSMComponent::HideFuriko()
{
	if (HidePoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("HidePoints 배열이 비어있습니다."));
		return;
	}

	// 랜덤 인덱스 선택
	const int32 Index = FMath::RandRange(0, HidePoints.Num() - 1);
	AFurikoHidePoint* Target = HidePoints[Index];

	if (Target && Owner) // 위치 이동
	{
		SetActorLocation(Target->GetActorLocation());

		// CreateUI <푸리코와 놀자!>: 퀘스트명 ui 띄울까 말까 윤호오빠랑 얘기해볼 것

		UE_LOG(LogTemp, Log, TEXT("Furiko가 HidePoint %s 로 순간이동했습니다."), *Target->GetName());
	}

}

void UFurikoFSMComponent::SetActorLocation(FVector InLocation)
{
	if (!Owner) return;

	// 순간 이동
	Owner->SetActorLocation(InLocation, false, nullptr, ETeleportType::TeleportPhysics);

	UE_LOG(LogTemp, Log, TEXT("Furiko가 HidePoint로 이동했습니다: %s"), *InLocation.ToString());
}
