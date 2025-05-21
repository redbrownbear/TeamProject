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

	int32 CurrentStrollIndex = 0;
	FVector TargetLocation = StrollPath->GetSplinePointLocation(CurrentStrollIndex);
	FVector CurrentLocation = Owner->GetActorLocation();

	// 이동
	MoveToLocation(TargetLocation);

	// 회전: 현재 위치 → TargetLocation 방향을 기준으로
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FRotator NpcRotation;
	if (!Direction.IsNearlyZero())
	{
		NpcRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	}

	// 부드러운 회전 적용
	FRotator SmoothRotation = FMath::RInterpTo(Owner->GetActorRotation(), NpcRotation, DeltaTime, 5.f);
	Owner->SetActorRotation(SmoothRotation);

	// 도착 체크
	/*bool bIsNear = FVector::Dist(CurrentLocation, TargetLocation) < 50.f;*/
	const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), TargetLocation, 500.f);
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
		Controller->GetConversationManager()->StartConversation(Owner, Player);
	}

	// 퀘스트 수락 버튼 클릭 시
	/*bool bAcceptQuest = Dialogue->OnClicked();
	if (bAcceptQuest)
	{
		ChangeState(ENpcState::Hide);
	}*/

	// 퀘스트 안 함 + 대화 끝남(UI 삭제)
	/*if (!bAcceptQuest && !Dialogue->GetDialogueState())
	{
		ChangeState(ENpcState::Idle);
	}*/
}

void UFurikoFSMComponent::UpdateHide(float DeltaTime)
{
	Super::UpdateHide(DeltaTime);
}

void UFurikoFSMComponent::UpdatePlay(float DeltaTime)
{
	// 술래잡기: 시간 되면 추가
}

