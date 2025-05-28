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

		FVector NewLocation = SurfacePoint + SurfaceNormal * 20;

		Player_C->SetActorRotation(FRotator(Player_Rot));

		Player_C->SetActorLocation(NewLocation);

		SetMovementClimb();

		

		return true;
	}
	

	SetClimbMode(false);

	return false;
}

void UPlayerMovementComponent::SetClimbMode(bool _bool)
{
	
	MovementMode = _bool ? MOVE_Flying : MOVE_Walking;

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

	bOrientRotationToMovement = !_bool;

	AnimInst->bIsCliming = _bool;

	bIsClimbing = _bool;

	Player_C->GetSpringArm()->bUsePawnControlRotation = !_bool;

}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


