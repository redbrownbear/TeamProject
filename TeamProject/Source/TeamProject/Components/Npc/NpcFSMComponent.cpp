#include "NpcFSMComponent.h"
#include "Controller/Npc/NpcController.h"
#include "Character/Npc/Npc.h"
#include "Navigation/PathFollowingComponent.h"

UNpcFSMComponent::UNpcFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNpcFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UNpcFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UNpcFSMComponent::HandleState(float DeltaTime)
{
	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case ENpcState::Stroll:
		UpdateStroll(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	default:
		break;
	}
}

void UNpcFSMComponent::ChangeState(ENpcState NewState)
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
	default:
		break;
	}
	eCurrentState = NewState;
}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
	// 기본 상태
}

void UNpcFSMComponent::UpdateStroll(float DeltaTime)
{
	if (!Owner || !StrollPathActor) return;

	const FVector Current = Owner->GetActorLocation();
	const FVector Target = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);

	MoveToLocation(Target);

	// 목표 지점에 도달했다면 다음 포인트로
	if (FVector::Dist(Current, Target) <= AcceptanceRadius)
	{
		CurrentStrollIndex = (CurrentStrollIndex + 1) % StrollPathActor->GetSplineMaxIndex();
	}

	UE_LOG(LogTemp, Warning, TEXT("UNpcFSMComponent::UpdateStroll // %f %f %f"), Current.X, Current.Y, Current.Z);

	// 플레이어가 캐릭터와 접촉해서 키를 입력받았을 때 UpdateTalk로 전환

	if (NpcController->bTalk) // 상호작용 키 입력이 true일 때 활성화
	{
		SetNpcState(ENpcState::Talk);
	}



	//if (!Owner || !StrollPathActor) return;

	//const FVector Current = Owner->GetActorLocation();
	//const FVector Target = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);

	//UE_LOG(LogTemp, Warning, TEXT("UNpcFSMComponent::UpdateStroll // %f %f %f"), Current.X, Current.Y, Current.Z);

	//// 목표 지점에 도달했다면 다음 포인트로
	//if (FVector::Dist(Current, Target) <= AcceptanceRadius)
	//{
	//	CurrentStrollIndex = (CurrentStrollIndex + 1) % StrollPathActor->GetSplineMaxIndex();
	//}

	//const FVector NextTarget = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);
	//MoveToLocation(NextTarget);

	//// 스플라인 따라 이동
	//if (NpcController)
	//{
	//	NpcController->Stroll();
	//}

	//// 플레이어가 캐릭터와 접촉해서 키를 입력받았을 때 UpdateTalk로 전환

	//if (NpcController->bTalk) // 상호작용 키 입력이 true일 때 활성화
	//{
	//	SetNpcState(ENpcState::Talk);
	//}
}

void UNpcFSMComponent::UpdateTalk(float DeltaTime)
{
	// Play Montage
}

void UNpcFSMComponent::MoveToLocation(const FVector& InLocation)
{
	NpcController = Cast<ANpcController>(Owner->GetController());
	if (NpcController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(InLocation);
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

		FNavPathSharedPtr NavPath;
		NpcController->MoveTo(MoveRequest, &NavPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NpcFSMComponent::MoveToLocation // No AIController"));
	}
}
