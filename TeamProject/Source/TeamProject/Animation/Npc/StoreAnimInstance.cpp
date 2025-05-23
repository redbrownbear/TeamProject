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
		checkf(false, TEXT("UStoreAnimInstance를 사용하려면 소유권자가 Pawn이어야 합니다."));
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
		break;
	case ENpcState::Talk:
		bIsIdle = false;
		break;
	case ENpcState::Sell:
		bIsSell = true;
		break;
	default:
		break;
	}
}
