#include "FurikoAnimInstance.h"
#include "Actors/Npc/Npc.h"
//#include "Components/FSMComponent/Npc/NpcFSMComponent.h"

UFurikoAnimInstance::UFurikoAnimInstance()
	:Super()
{		
}

void UFurikoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();

	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UFurikoAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	ANpc* Npc = Cast<ANpc>(Pawn);
	FSMComponent = Cast<UFurikoFSMComponent>(Npc->GetFSMComponent());
}

void UFurikoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ANpc* Npc = Cast<ANpc>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UFurikoFSMComponent>(Npc->GetFSMComponent());
	}

	if (!FSMComponent) { return; }

	if (APawn* Pawn = TryGetPawnOwner())
	{
		Speed = Pawn->GetVelocity().Size2D(); 
	} 

	eCurrentState = FSMComponent->GetNpcState();

	bIsIdle = false;
	bIsSit = false;
	bIsStand = false;
	bIsWalk = false;
	bIsRun = false;
	bIsTalk = false;
	bIsHide = false;

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		bIsIdle = true;	
		break;
	case ENpcState::Sit:
		bIsSit = true;
		break;
	case ENpcState::Stand:
		bIsStand = true;
		break;
	case ENpcState::Run:
		bIsRun = true;
		break;
	case ENpcState::Talk:
		bIsTalk = true;
		break;
	case ENpcState::Hide:
		bIsHide = true;
		break;
	default:
		break;
	}
}
