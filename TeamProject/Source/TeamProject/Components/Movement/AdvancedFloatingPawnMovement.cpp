// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Movement/AdvancedFloatingPawnMovement.h"

#include "AI/Navigation/NavigationDataInterface.h"
#include "AI/NavigationSystemBase.h"

float GetAxisDeltaRotation2(float InAxisRotationRate, float DeltaTime)
{
	// Values over 360 don't do anything, see FMath::FixedTurn. However we are trying to avoid giant floats from overflowing other calculations.
	return (InAxisRotationRate >= 0.f) ? FMath::Min(InAxisRotationRate * DeltaTime, 360.f) : 360.f;
}

UAdvancedFloatingPawnMovement::UAdvancedFloatingPawnMovement()
	: bOrientRotationToMovement(true)
{
	RotationRate = FRotator(0.f, 0.0f, 0.0f);
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

	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Super::TickComponent를 아래로 옮겨놔서 그 부모의 TickComponent를 호출
	UPawnMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	{
		const AController* Controller = PawnOwner->GetController();
		if (Controller && Controller->IsLocalController())
		{
			// AI Move To 에서 Velocity가 커지는 이슈가 있어서 속도 제한
			// See: UPathFollowingComponent::FollowPathSegment
			if (IsExceedingMaxSpeed(MaxSpeed) == true)
			{
				Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
			}

			static const FVector GravityDirection = FVector::UpVector;
			static const FVector Gravity = -GravityDirection * 980.0;
			if (CheckFalling(DeltaTime))
			{
				Velocity = NewFallVelocity(Velocity, Gravity, FallingDeltaTime);
			}
			else if (!Velocity.IsNearlyZero())
			{
				// 통상의 경우 미세 중력을 적용해서 공중으로 뜨지 않도록 처리
				Velocity = NewFallVelocity(Velocity, Gravity, DeltaTime * 5.f);
			}

			LimitWorldBounds();
			bPositionCorrected = false;

			// Move actor
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
					// Try to slide the remaining distance along the surface.
					SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
				}

				// Update velocity
				// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
				if (!bPositionCorrected)
				{
					const FVector NewLocation = UpdatedComponent->GetComponentLocation();
					Velocity = ((NewLocation - OldLocation) / DeltaTime);
				}
			}

			AccelerationAdvance = Velocity.GetSafeNormal();

			PhysicsRotation(DeltaTime);

			// Finalize
			UpdateComponentVelocity();
		}
	}

}