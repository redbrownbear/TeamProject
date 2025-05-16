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
	// �⺻ ����
}

void UNpcFSMComponent::UpdateStroll(float DeltaTime)
{
	if (!Owner || !StrollPathActor) return;

	const FVector Current = Owner->GetActorLocation();
	const FVector Target = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);

	MoveToLocation(Target);

	// ��ǥ ������ �����ߴٸ� ���� ����Ʈ��
	if (FVector::Dist(Current, Target) <= AcceptanceRadius)
	{
		CurrentStrollIndex = (CurrentStrollIndex + 1) % StrollPathActor->GetSplineMaxIndex();
	}

	UE_LOG(LogTemp, Warning, TEXT("UNpcFSMComponent::UpdateStroll // %f %f %f"), Current.X, Current.Y, Current.Z);

	// �÷��̾ ĳ���Ϳ� �����ؼ� Ű�� �Է¹޾��� �� UpdateTalk�� ��ȯ

	if (NpcController->bTalk) // ��ȣ�ۿ� Ű �Է��� true�� �� Ȱ��ȭ
	{
		SetNpcState(ENpcState::Talk);
	}



	//if (!Owner || !StrollPathActor) return;

	//const FVector Current = Owner->GetActorLocation();
	//const FVector Target = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);

	//UE_LOG(LogTemp, Warning, TEXT("UNpcFSMComponent::UpdateStroll // %f %f %f"), Current.X, Current.Y, Current.Z);

	//// ��ǥ ������ �����ߴٸ� ���� ����Ʈ��
	//if (FVector::Dist(Current, Target) <= AcceptanceRadius)
	//{
	//	CurrentStrollIndex = (CurrentStrollIndex + 1) % StrollPathActor->GetSplineMaxIndex();
	//}

	//const FVector NextTarget = StrollPathActor->GetSplinePointLocation(CurrentStrollIndex);
	//MoveToLocation(NextTarget);

	//// ���ö��� ���� �̵�
	//if (NpcController)
	//{
	//	NpcController->Stroll();
	//}

	//// �÷��̾ ĳ���Ϳ� �����ؼ� Ű�� �Է¹޾��� �� UpdateTalk�� ��ȯ

	//if (NpcController->bTalk) // ��ȣ�ۿ� Ű �Է��� true�� �� Ȱ��ȭ
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
