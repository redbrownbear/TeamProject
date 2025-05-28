// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UPlayerMovementComponent::TrySetMoveClimb()
{
	FHitResult HitResult;
	if (ClimbingLineTrace(HitResult))
	{
		FRotator Normal_Rot = FRotationMatrix::MakeFromX(HitResult.ImpactNormal).Rotator();

		FRotator Player_Rot = GetOwner()->GetActorRotation();

		FVector SurfaceNormal = HitResult.ImpactNormal;

		Player_Rot.Yaw = Normal_Rot.Yaw + 180.f;

		Player_Rot.Pitch = -Normal_Rot.Pitch;

		FVector SurfacePoint = HitResult.ImpactPoint;

		FVector NewLocation = SurfacePoint + SurfaceNormal * 10;

		bOrientRotationToMovement = false;

		GetOwner()->SetActorRotation(FRotator(Player_Rot));

		GetOwner()->SetActorLocation(NewLocation);

		SetMovementClimb();
	}
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


