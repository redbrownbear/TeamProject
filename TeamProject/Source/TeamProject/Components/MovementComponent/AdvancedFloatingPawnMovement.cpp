// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"

#include "AI/Navigation/NavigationDataInterface.h"
#include "AI/NavigationSystemBase.h"

#include "Actors/Npc/Npc.h"

float GetAxisDeltaRotation2(float InAxisRotationRate, float DeltaTime)
{
	// Values over 360 don't do anything, see FMath::FixedTurn. However we are trying to avoid giant floats from overflowing other calculations.
	return (InAxisRotationRate >= 0.f) ? FMath::Min(InAxisRotationRate * DeltaTime, 360.f) : 360.f;
}

UAdvancedFloatingPawnMovement::UAdvancedFloatingPawnMovement()
	: bOrientRotationToMovement(true)
{
	RotationRate = FRotator(0.f, 360.0f, 0.0f);
}

FRotator UAdvancedFloatingPawnMovement::GetDeltaRotation(float DeltaTime) const
{
	return FRotator(GetAxisDeltaRotation2(RotationRate.Pitch, DeltaTime), GetAxisDeltaRotation2(RotationRate.Yaw, DeltaTime), GetAxisDeltaRotation2(RotationRate.Roll, DeltaTime));
}

void UAdvancedFloatingPawnMovement::PhysicsRotation(float DeltaTime)
{
	if (!bOrientRotationToMovement) { return; }

	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	CurrentRotation.DiagnosticCheckNaN(TEXT("UAdvenceFloatingPawnMovement::PhysicsRotation(): CurrentRotation"));

	FRotator DeltaRot = GetDeltaRotation(DeltaTime);
	DeltaRot.DiagnosticCheckNaN(TEXT("UAdvenceFloatingPawnMovement::PhysicsRotation(): GetDeltaRotation"));

	FRotator DesiredRotation = CurrentRotation;
	if (bOrientRotationToMovement)
	{
		DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);
	}

	// const bool bWantsToBeVertical = ShouldRemainVertical();
	{
		DesiredRotation.Pitch = 0.f;
		DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
		DesiredRotation.Roll = 0.f;
	}

	// Accumulate a desired new rotation.
	const float AngleTolerance = 1e-3f;
	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// If we'd be prevented from becoming vertical, override the non-yaw rotation rates to allow the character to snap upright
		//if (CharacterMovementCVars::bPreventNonVerticalOrientationBlock && bWantsToBeVertical)
		{
			if (FMath::IsNearlyZero(DeltaRot.Pitch))
			{
				DeltaRot.Pitch = 360.0;
			}
			if (FMath::IsNearlyZero(DeltaRot.Roll))
			{
				DeltaRot.Roll = 360.0;
			}
		}

		{
			// PITCH
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// YAW
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ROLL
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}
		}
		// Set the new rotation.
		DesiredRotation.DiagnosticCheckNaN(TEXT("UAdvanceFloatingPawnMovement::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}
}

bool UAdvancedFloatingPawnMovement::CheckFalling(float DeltaTime)
{
	if (!bIsGravity) return false;


	APawn* OwningPawn = Cast<APawn>(GetOwner());

	const FVector OldLocation = GetActorFeetLocation();

	FVector DesiredMove = Velocity;
	DesiredMove.Z = 0.f;
	const FVector DeltaMove = DesiredMove * DeltaTime;
	FVector AdjustedDest = OldLocation + DeltaMove;

	//const INavigationDataInterface* NavData = FNavigationSystem::GetNavDataForActor(*OwningPawn);
	const FNavAgentProperties& AgentProps = OwningPawn->GetNavAgentPropertiesRef();
	//const float SearchRadius = AgentProps.AgentRadius * 2.0f;
	const float SearchHeight = AgentProps.AgentHeight * AgentProps.NavWalkingSearchHeightScale * 0.4f;
	//FNavLocation DestNavLocation;
	bool bFoundFloor = false;//NavData->ProjectPoint(AdjustedDest, DestNavLocation, FVector(SearchRadius, SearchRadius, SearchHeight));
	//if (!bFoundFloor)
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		FVector Start = OldLocation;
		FVector End = Start - FVector(0.0f, 0.0f, SearchHeight);

		bFoundFloor = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	}

	if (bFoundFloor)
	{
		FallingDeltaTime = 0.f;
	}
	else
	{
		FallingDeltaTime += DeltaTime;
	}

	bFalling = !bFoundFloor;
	return bFalling;
}

bool UAdvancedFloatingPawnMovement::IsFalling() const
{
	return bFalling;
}

bool UAdvancedFloatingPawnMovement::IsMovingOnGround() const
{
	return !bFalling;
}

FVector UAdvancedFloatingPawnMovement::NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const
{
	FVector Result = InitialVelocity;

	if (DeltaTime > 0.f)
	{
		// Apply gravity.
		Result += Gravity * DeltaTime;

		// Don't exceed terminal velocity.
		const float TerminalLimit = 4000.f;// FMath::Abs(GetPhysicsVolume()->TerminalVelocity);
		if (Result.SizeSquared() > FMath::Square(TerminalLimit))
		{
			const FVector GravityDir = Gravity.GetSafeNormal();
			if ((Result | GravityDir) > TerminalLimit)
			{
				Result = FVector::PointPlaneProject(Result, FVector::ZeroVector, GravityDir) + GravityDir * TerminalLimit;
			}
		}
	}

	return Result;
}

FRotator UAdvancedFloatingPawnMovement::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const
{
	if (AccelerationAdvance.SizeSquared() < UE_KINDA_SMALL_NUMBER)
	{
		// AI path following request can orient us in that direction (it's effectively an acceleration)
		//if (bHasRequestedVelocity && RequestedVelocity.SizeSquared() > UE_KINDA_SMALL_NUMBER)
		//{
		//	return RequestedVelocity.GetSafeNormal().Rotation();
		//}

		// Don't change rotation if there is no acceleration.
		return CurrentRotation;
	}

	// Rotate toward direction of acceleration.
	return AccelerationAdvance.GetSafeNormal().Rotation();
}

void UAdvancedFloatingPawnMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// 1. 먼저 부모 클래스의 TickComponent를 호출하여 입력에 따른 Velocity를 계산합니다.
	//    이 시점에서 Velocity는 플레이어/AI의 입력에 의해 갱신됩니다.
	UPawnMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	// AController* Controller = PawnOwner->GetController(); // 컨트롤러 체크는 필요에 따라 계속 유지

	// 2. 컨트롤러가 로컬 컨트롤러일 때만 이동 및 중력 로직을 적용합니다.
	//    (보통 PawnOwner->IsLocallyControlled()를 사용합니다.)
	if (PawnOwner->IsLocallyControlled()) // 또는 Controller && Controller->IsLocalController()
	{
		// 3. AI Move To 에서 Velocity가 커지는 이슈 처리 (이전 로직 유지)
		if (IsExceedingMaxSpeed(MaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		static const FVector GravityDirection = FVector::UpVector;
		static const FVector Gravity = -GravityDirection * 980.0;

		// 4. 낙하 상태를 체크하고 중력을 적용합니다.
		if (CheckFalling(DeltaTime))
		{
			// 낙하 중일 때는 계속 중력 가속도를 적용합니다.
			Velocity = NewFallVelocity(Velocity, Gravity, FallingDeltaTime);
		}
		else // 땅에 닿아 있을 때
		{
			// 움직임 입력이 없는 경우에만 Velocity를 0으로 설정하여 잔여 움직임을 제거합니다.
			// Small_Number보다 작은 경우 멈춘 것으로 간주합니다.
			if (Velocity.SizeSquared() < KINDA_SMALL_NUMBER) // 또는 Velocity.IsNearlyZero(SMALL_NUMBER)
			{
				Velocity = FVector::ZeroVector;
			}
			else
			{
				// 땅에 닿아 있고 움직임 입력이 있는 경우, 미세 중력을 적용하지 않습니다.
				// 만약 여전히 '떠오르는' 문제가 있다면, 아주 미세한 중력만 적용하도록 조건을 추가할 수 있습니다.
				// 예를 들어, Z축 속도가 양수이고 매우 작을 때만 중력을 적용하는 등.
				// 현재 코드에서 미세 중력은 IsFalling() == false && !Velocity.IsNearlyZero() 일 때 적용되었는데
				// 땅에 닿아있다면, Velocity가 완전히 0이 아닌 이상 기본적으로 중력을 적용할 필요가 없습니다.
				// 이 부분을 제거하거나 매우 신중하게 적용해야 합니다.
				// Velocity = NewFallVelocity(Velocity, Gravity, DeltaTime * 5.f); // 이 줄을 제거하거나 조건부로 변경 고려
			}
		}

		LimitWorldBounds();
		bPositionCorrected = false;

		// 5. 계산된 Velocity를 사용하여 액터를 이동합니다.
		FVector Delta = Velocity * DeltaTime;

		if (!Delta.IsNearlyZero(1e-6f))
		{
			const FVector OldLocation = UpdatedComponent->GetComponentLocation();
			const FQuat Rotation = UpdatedComponent->GetComponentQuat();

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, DeltaTime, Delta);
				SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}

			if (!bPositionCorrected)
			{
				const FVector NewLocation = UpdatedComponent->GetComponentLocation();
				Velocity = ((NewLocation - OldLocation) / DeltaTime);
			}
		}

		// 6. 회전 처리를 위한 AccelerationAdvance 갱신
		AccelerationAdvance = Velocity.GetSafeNormal(); // Velocity가 0이 아니면 정상적인 방향 벡터를 가집니다.

		// 7. 회전 처리
		PhysicsRotation(DeltaTime);

		// 8. 최종 Velocity 업데이트
		UpdateComponentVelocity();
	}
}