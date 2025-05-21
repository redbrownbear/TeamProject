#include "FurikoFSMComponent.h"
#include "Actors/Npc/Npc.h"

#include "Actors/StrollPath/StrollPath.h"
#include "Components/SplineComponent.h"

#include "UI/NpcDialogue/NPCDialogue.h"
#include "Animation/Npc/ConversationManagerComponent.h"

UFurikoFSMComponent::UFurikoFSMComponent()
{
	eCurrentState = ENpcState::Stroll;
}

void UFurikoFSMComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFurikoFSMComponent::HandleState(float DeltaTime)
{
	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
	case ENpcState::Stroll:
		UpdateStroll(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	case ENpcState::Hide:
		UpdateHide(DeltaTime);
		break;
	case ENpcState::Play: // 술래잡기: 시간 되면 추가
		UpdatePlay(DeltaTime); 
		break;
	default:
		break;
	}
}

void UFurikoFSMComponent::ChangeState(ENpcState NewState)
{
	if (eCurrentState == NewState) { return; }
	switch (NewState)
	{
	case ENpcState::Idle:
		break;
	case ENpcState::Stroll:
		break;
	case ENpcState::Talk:
		break;
	case ENpcState::Hide:
		break;
	case ENpcState::Play: // 술래잡기: 시간 되면 추가
		break;
	default:
		break;
	}
	eCurrentState = NewState;
}

void UFurikoFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UFurikoFSMComponent::UpdateStroll(float DeltaTime)
{	
	Super::UpdateStroll(DeltaTime);

	AStrollPath* StrollPath = Owner->GetStrollPath();
	if (!StrollPath) return;

	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector TargetLocation = StrollPath->GetSplinePointLocation(CurrentStrollIndex);

	// 이동
	MoveToLocation(TargetLocation);

	// 회전 보간 속도 계산
	UFloatingPawnMovement* MoveComp = Cast<UFloatingPawnMovement>(Owner->GetMovementComponent());
	float MovementSpeed = (MoveComp != nullptr) ? MoveComp->MaxSpeed : 300.f;
	float RotationInterpSpeed = MovementSpeed / 50.f; // 튜닝값 (작을수록 천천히 회전)

	// 방향 계산 (다음 인덱스를 X축 정면으로 바라보게 회전)
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	if (!Direction.IsNearlyZero())
	{
		FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		FRotator CurrentRotation = Owner->GetActorRotation();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);

		Owner->SetActorRotation(SmoothRotation);
	}

	// 도착 체크
	const float Distance = FVector::Dist(CurrentLocation, TargetLocation);
	if (Distance < 100.f)
	{
		++CurrentStrollIndex;
		if (CurrentStrollIndex >= StrollPath->GetSplineMaxIndex())
		{
			CurrentStrollIndex = 0;
		}
	}
	
	//// 목표 위치 구하기
	//FVector Location = FVector();

	//if (AStrollPath* StrollPath = Owner->GetStrollPath())
	//{
	//	Location = StrollPath->GetSplinePointLocation(CurrentStrollIndex);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::UpdateStroll // No StrollPath"));
	//	check(false);
	//}

	//// 이동 
	//MoveToLocation(Location);

	//// 다음 PatrolIndex 구하기
	//const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), Location, 100.f);


	//if (bIsNear)
	//{
	//	++CurrentStrollIndex;

	//	if (AStrollPath* StrollPath = Owner->GetStrollPath())
	//	{
	//		if (CurrentStrollIndex >= StrollPath->GetSplineMaxIndex())
	//		{
	//			CurrentStrollIndex = 0;
	//		}
	//	}
	//}
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
	
}

void UFurikoFSMComponent::UpdatePlay(float DeltaTime)
{
	// 술래잡기: 시간 되면 추가
};
