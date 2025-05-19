#include "FurikoAnimInstance.h"
#include "Actors/Npc/Npc.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"

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

	/*UE_LOG(LogTemp, Warning, TEXT("AnimInstance // Speed: %.1f | State: %d | bIsRun: %s"),
		Speed,
		static_cast<uint8>(eCurrentState),
		bIsRun ? TEXT("true") : TEXT("false"));*/

	eCurrentState = FSMComponent->GetNpcState();

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		bIsIdle = true;
		bIsSit = false;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = false;		
		break;
	case ENpcState::Sit:
		bIsIdle = false;
		bIsSit = true;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = false;
		break;
	case ENpcState::Stand:
		bIsIdle = false;
		bIsSit = false;
		bIsStand = true;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = false;
		break;
	case ENpcState::Stroll:
		bIsIdle = false;
		bIsSit = false;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = true;
		bIsTalk = false;
		bIsHide = false;
		break;
	case ENpcState::Talk:
		bIsIdle = false;
		bIsSit = false;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = true;
		bIsHide = false;
		break;
	case ENpcState::Hide:
		bIsIdle = false;
		bIsSit = false;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = true;
		break;
	default:
		break;
	}
}
