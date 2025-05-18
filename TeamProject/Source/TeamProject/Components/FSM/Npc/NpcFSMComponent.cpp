#include "NpcFSMComponent.h"
//#include "Controller/Npc/NpcController.h"
#include "Character/Npc/Npc.h"
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
	
	/*Owner = Cast<ANpc>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::BeginPlay - Owner is not ANpc"));
		return;
	}*/

	/*if (!StrollPathActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No StrollPathActor"));
	}*/

	/*if (!Owner->GetStrollPath())
	{
		TArray<AActor*> FoundPaths;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStrollPath::StaticClass(), FoundPaths);

		if (FoundPaths.Num() > 0)
		{
			AStrollPath* FoundPath = Cast<AStrollPath>(FoundPaths[0]);
			Owner->SetStrollPath(FoundPath); 
		}
	}*/
}

void UNpcFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner) {return;}
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
	/*if (Owner->GetStrollPath())
	{
		UpdateStroll(DeltaTime);
	}*/
}

void UNpcFSMComponent::UpdateStroll(float DeltaTime)
{
	//if (!Owner || !StrollPathActor) { return; }
	
	// 목표 위치 구하기
	FVector Location = FVector();

	if (AStrollPath* StrollPath = Owner->GetStrollPath())
	{
		Location = StrollPath->GetSplinePointLocation(CurrentStrollIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::UpdateStroll // No StrollPath"));
		check(false);
	}

	// 이동 
	MoveToLocation(Location);

	// 다음 PatrolIndex 구하기
	const bool bIsNear = FVector::PointsAreNear(Owner->GetActorLocation(), Location, 150.f);

	if (bIsNear)
	{
		++CurrentStrollIndex;

		if (AStrollPath* StrollPath = Owner->GetStrollPath())
		{
			if (CurrentStrollIndex >= StrollPath->GetSplineMaxIndex())
			{
				CurrentStrollIndex = 0;
			}
		}
	}

	/*if (NpcController->bTalk) 
	{
		SetNpcState(ENpcState::Talk);
	}*/
}

void UNpcFSMComponent::UpdateTalk(float DeltaTime)
{
	// Play Montage
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
