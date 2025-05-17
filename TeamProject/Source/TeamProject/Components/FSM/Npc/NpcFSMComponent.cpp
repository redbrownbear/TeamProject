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
	
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::BeginPlay - Owner is null"));
		return;
	}

	if (!StrollPathActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No StrollPathActor"));
	}
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
	
}

void UNpcFSMComponent::UpdateStroll(float DeltaTime)
{
	if (!Owner || !StrollPathActor) return;

	const FVector Current = Owner->GetActorLocation();
	const FVector Target = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);

	MoveToLocation(Target);

	if (FVector::Dist(Current, Target) <= AcceptanceRadius)
	{
		CurrentStrollIndex = (CurrentStrollIndex + 1) % StrollPathActor->GetSplineMaxIndex();
	}

	//E_LOG(LogTemp, Warning, TEXT("UNpcFSMComponent::UpdateStroll // %f %f %f"), Current.X, Current.Y, Current.Z);

	/*UE_LOG(LogTemp, Warning, TEXT("Target: %s, Current: %s, Dist: %.1f"),
		*Target.ToString(),
		*Current.ToString(),
		FVector::Dist(Current, Target));*/


	if (NpcController->bTalk) 
	{
		SetNpcState(ENpcState::Talk);
	}
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
