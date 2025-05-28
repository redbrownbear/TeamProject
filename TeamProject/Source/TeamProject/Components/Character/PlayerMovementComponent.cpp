// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Actors/Character/PlayerCharacter.h"

UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer)
{

	BrakingDecelerationFlying = 10000.f;
	AirControl = 0.f;
	
}

bool UPlayerMovementComponent::ClimbingLineTrace(FHitResult& HitResult)
{
	AActor* ComponentOwner = GetOwner();
	
	const FVector OwnerLocation = ComponentOwner->GetActorLocation();
	const FRotator OwnerLotator = ComponentOwner->GetActorRotation();


	FVector OwnerForwardVector = ComponentOwner->GetActorForwardVector();
	
	FVector Start = OwnerLocation;
	FVector End = OwnerLocation + OwnerForwardVector * 50;

	

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ComponentOwner);

	bool Returnbool = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);

	DrawDebugLine(GetWorld(), Start, End,FColor::Red,false, 2.f);

	return Returnbool;
}

bool UPlayerMovementComponent::TrySetMoveClimb()
{
	FHitResult HitResult;

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	if (ClimbingLineTrace(HitResult))
	{
		
		

		FRotator Normal_Rot = FRotationMatrix::MakeFromX(HitResult.ImpactNormal).Rotator();

		FRotator Player_Rot = Player_C->GetActorRotation();

		FVector SurfaceNormal = HitResult.ImpactNormal;

		Player_Rot.Yaw = Normal_Rot.Yaw + 180.f;

		Player_Rot.Pitch = -Normal_Rot.Pitch;

		FVector SurfacePoint = HitResult.ImpactPoint;

		FVector NewLocation = SurfacePoint + SurfaceNormal * 5;

		bOrientRotationToMovement = false;

		Player_C->SetActorRotation(FRotator(Player_Rot));

		Player_C->SetActorLocation(NewLocation);

		SetMovementClimb();

		

		return true;
	}
	MovementMode = MOVE_Walking;

	Cast<UPlayerAnimInstance>((Player_C->GetMesh()->GetAnimInstance()))->bIsCliming = false;

	bIsClimbing = false;

	bOrientRotationToMovement = true;

	return false;
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


