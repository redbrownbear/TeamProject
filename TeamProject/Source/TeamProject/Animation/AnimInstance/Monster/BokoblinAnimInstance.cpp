// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/Monster/BokoblinAnimInstance.h"
#include "Components/FSMComponent/Monster/BokoblinFSMComponent.h"

#include "Actors/Monster/PawnMonster.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

#include "Subsystem/TimeManager.h"

UBokoblinAnimInstance::UBokoblinAnimInstance()
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

	APawnMonster* Monster = Cast<APawnMonster>(Pawn);
	FSMComponent = Cast<UBokoblinFSMComponent>(Monster->GetFSMComponent());
	TimeManager = Monster->GetTimeManagerSubsystem();
}

void UBokoblinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	float CustumDeltaTime = 0.016f;
	float TimeManagerScale = 1.f;

	if (TimeManager)
	{
		CustumDeltaTime = TimeManager->GetCustomDeltaTime();
		TimeManagerScale = TimeManager->GetTimeScale();
	}
	Super::NativeUpdateAnimation(CustumDeltaTime);

	if (APawnMonster* Monster = Cast<APawnMonster>(TryGetPawnOwner()))
	{
		FSMComponent = Cast<UBokoblinFSMComponent>(Monster->GetFSMComponent());
	}

	if (!FSMComponent) return;

	UAnimMontage* CurrentMontage = GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		float RateScale = CurrentMontage->RateScale;
		Montage_SetPlayRate(CurrentMontage, RateScale * TimeManagerScale);
	}

	const EMonsterState eMonsterState = FSMComponent->GetCurrentState();

	bIsIdle = false;
	bIsWalk = false;
	bIsRun = false;
	bIsSearch = false;
	bIsFire = false;
	bIsEat = false;
	bIsDance = false;
	bIsSignal= false;
	bIsSuspicious = false;
	bIsAimingBow = false;

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
		bIsWalk = true;
		break;
	case EMonsterState::Signal:
		bIsSignal = true;
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:
	{
		if (APawnMonster* Monster = Cast<APawnMonster>(TryGetPawnOwner()))
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
	case EMonsterState::FindWeapon:
		bIsRun = true;
		break;
	case EMonsterState::AimingBow:
		bIsAimingBow = true;
		break;
	case EMonsterState::Dead:
		break;
	case EMonsterState::End:
		break;
	default:
		break;	
	}
}
