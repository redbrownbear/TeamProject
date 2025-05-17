// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Monster/MoriblinAnimInstance.h"

#include "Components/FSMComponent/MoriblinFSMComponent.h"

#include "Actors/Monster/Monster.h"


UMoriblinAnimInstance::UMoriblinAnimInstance()
{
}

void UMoriblinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UMoriblinAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	AMonster* Monster = Cast<AMonster>(Pawn);
	FSMComponent = Cast<UMoriblinFSMComponent>(Monster->GetFSMComponent());

}

void UMoriblinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (AMonster* Monster = Cast<AMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UMoriblinFSMComponent>(Monster->GetFSMComponent());
	}

	if (!FSMComponent) return;

	const EMonsterState eMonsterState = FSMComponent->GetMonsterState();
	switch (eMonsterState)
	{
	case EMonsterState::Idle:
		bIsIdle = true;
		bIsWalk = false;
		bIsRun = false;
		bIsSearch = false;
		bIsFire = false;
		bIsEat = false;
		bIsDance = false;
		break;
	case EMonsterState::Patrol:
		bIsIdle = false;
		bIsWalk = true;
		bIsRun = false;
		bIsSearch = false;
		bIsFire = false;
		bIsEat = false;
		bIsDance = false;
		break;
	case EMonsterState::Suspicious:
		bIsIdle = false;
		bIsWalk = false;
		bIsRun = false;
		bIsSearch = true;
		bIsFire = false;
		bIsEat = false;
		bIsDance = false;
		break;

	case EMonsterState::Fire:
		bIsIdle = false;
		bIsWalk = false;
		bIsRun = false;
		bIsSearch = false;
		bIsFire = true;
		bIsEat = false;
		bIsDance = false;
		break;
	case EMonsterState::Eat:
		bIsIdle = false;
		bIsWalk = false;
		bIsRun = false;
		bIsSearch = false;
		bIsFire = false;
		bIsEat = true;
		bIsDance = false;
		break;
	case EMonsterState::Dance:
		bIsIdle = false;
		bIsWalk = false;
		bIsRun = false;
		bIsSearch = false;
		bIsFire = false;
		bIsEat = false;
		bIsDance = true;
		break;
	case EMonsterState::End:
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:
		break;
	case EMonsterState::Dead:
		break;
	default:
		break;
	}
}
