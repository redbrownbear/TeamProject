#include "NpcFSMComponent.h"
#include "Actors/Npc/Npc.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

UNpcFSMComponent::UNpcFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	eCurrentState = ENpcState::Stroll;
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
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::HandleState // No Owner"));
		check(false);
		return;
	}

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

	UE_LOG(LogTemp, Warning, TEXT("eCurrentState = %s"), *UEnum::GetValueAsString(eCurrentState));
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

	UE_LOG(LogTemp, Warning, TEXT("eCurrentState = %s"), *UEnum::GetValueAsString(eCurrentState));
}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
	if (eCurrentState != ENpcState::Idle)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Idle'"));
		return;
	}
}

void UNpcFSMComponent::UpdateStroll(float DeltaTime)
{	
	if (eCurrentState != ENpcState::Stroll)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Stroll'"));
		return;
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
	//const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), Location, 300.f);


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

void UNpcFSMComponent::UpdateTalk(float DeltaTime)
{
	if (eCurrentState != ENpcState::Talk)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Talk'"));
		return;
	}
	// Play Npc Talk Animation
	// Play Player Talk Animation
}

void UNpcFSMComponent::MoveToLocation(const FVector& InLocation)
{
	ANpcController* NpcController = Cast<ANpcController>(Owner->GetController());
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
		UE_LOG(LogTemp, Error, TEXT("NpcFSMComponent::MoveToLocation // No NpcController"));
		check(false);
	}
}
