#include "FurikoAnimInstance.h"
#include "Character/Npc/Npc.h"

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
		checkf(false, TEXT("UGPMarioAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
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

	UE_LOG(LogTemp, Warning, TEXT("AnimInstance // Speed: %.1f | State: %d | bIsRun: %s"),
		Speed,
		static_cast<uint8>(eCurrentState),
		bIsRun ? TEXT("true") : TEXT("false"));

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		bIsSit = false;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = false;		
		break;
	case ENpcState::Stroll:
		bIsSit = false;
		bIsStand = false;
		bIsWalk = false;
		bIsRun = true;
		bIsTalk = false;
		bIsHide = false;
		break;
	case ENpcState::Hide:
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
