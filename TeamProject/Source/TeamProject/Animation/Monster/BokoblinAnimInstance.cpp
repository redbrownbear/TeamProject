// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Monster/BokoblinAnimInstance.h"
#include "Components/FSMComponent/BokoblinFSMComponent.h"

#include "Actors/Monster/Monster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

UBokoblinAnimInstance::UBokoblinAnimInstance()
	:Super()
{
}

void UBokoblinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UBokoblinAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	AMonster* Monster = Cast<AMonster>(Pawn);
	FSMComponent = Cast<UBokoblinFSMComponent>(Monster->GetFSMComponent());
}

void UBokoblinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (AMonster* Monster = Cast<AMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UBokoblinFSMComponent>(Monster->GetFSMComponent());
	}

	if (!FSMComponent) return;

	const EMonsterState eMonsterState = FSMComponent->GetMonsterState();

	bIsIdle = false;
	bIsWalk = false;
	bIsRun = false;
	bIsSearch = false;
	bIsFire = false;
	bIsEat = false;
	bIsDance = false;
	bIsToDance = false;
	bIsSignal= false;
	bIsSuspicious = false;

	switch (eMonsterState)
	{
	case EMonsterState::Idle:
		bIsIdle = true;
		break;
	case EMonsterState::Patrol:
		bIsWalk = true;
		break;
	case EMonsterState::Suspicious:
		bIsSuspicious = true;
		break;
	case EMonsterState::Fire:
		bIsFire = true;
		break;
	case EMonsterState::Eat:
		bIsEat = true;
		break;
	case EMonsterState::Dance:
		bIsDance = true;
		break;
	case EMonsterState::ToDance:
		bIsToDance = true;
		break;
	case EMonsterState::Signal:
		bIsSignal = true;
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:
	{
		if (AMonster* Monster = Cast<AMonster>(TryGetPawnOwner()))
		{
			const double Speed = UKismetMathLibrary::VSizeXY(Monster->GetMovementComponent()->Velocity);
			if (FMath::IsNearlyZero(Speed))
			{
				bIsIdle = true;
			}
			else
			{
				bIsRun = true;
			}
		}
	}
		break;
	case EMonsterState::Dead:
		break;
	case EMonsterState::End:
		break;
	default:
		break;	
	}
}
