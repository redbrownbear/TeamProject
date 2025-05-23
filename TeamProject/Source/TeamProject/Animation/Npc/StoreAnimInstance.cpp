#include "Animation/Npc/StoreAnimInstance.h"
#include "Actors/Npc/Npc.h"

UStoreAnimInstance::UStoreAnimInstance()
	:Super()
{
}

void UStoreAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();

	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UStoreAnimInstance�� ����Ϸ��� �������ڰ� Pawn�̾�� �մϴ�."));
		return;
	}
	else if (!Pawn) { return; }

	ANpc* Npc = Cast<ANpc>(Pawn);
	FSMComponent = Cast<UStoreFSMComponent>(Npc->GetFSMComponent());
}

void UStoreAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ANpc* Npc = Cast<ANpc>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UStoreFSMComponent>(Npc->GetFSMComponent());
	}

	if (!FSMComponent) { return; }

	eCurrentState = FSMComponent->GetNpcState();

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		bIsIdle = true;
		bIsTalk = false;
		bIsSell = false;
		break;
	case ENpcState::Talk:
		bIsIdle = false;
		bIsTalk = true;
		bIsSell = false;
		break;
	case ENpcState::Sell:
		bIsIdle = false;
		bIsTalk = false;
		bIsSell = true;
		break;
	default:
		break;
	}
}
