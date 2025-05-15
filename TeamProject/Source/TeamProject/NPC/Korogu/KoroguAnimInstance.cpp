#include "KoroguAnimInstance.h"

UKoroguAnimInstance::UKoroguAnimInstance()
{
}

void UKoroguAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UGPMarioAnimInstance�� ����Ϸ��� �������ڰ� Pawn�̿��� �մϴ�."));
		return;
	}
	else if (!Pawn) { return; }

	/*AKorogu* Korogu = Cast<AKorogu>(Pawn);
	FSMComponent = Cast<UKoroguFSMComponent>(Korogu->GetFSMComponent());*/
}

void UKoroguAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!FSMComponent) return;

	//const EKoroguState eKoroguState = FSMComponent->GetKoroguState();

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		bIsIdle = true;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = false;		
		break;
	case ENpcState::Patrol:
		bIsIdle = false;
		bIsWalk = true;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = false;
		break;
	case ENpcState::Hide:
		bIsIdle = false;
		bIsWalk = false;
		bIsRun = false;
		bIsTalk = false;
		bIsHide = true;
		break;
	default:
		break;
	}
}
