// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer)
{


}

bool UPlayerMovementComponent::ClimbingLineTrace(FHitResult& HitResult)
{
	AActor* ComponentOwner = GetOwner();
	
	const FVector OwnerLocation = ComponentOwner->GetActorLocation();
	const FRotator OwnerLotator = ComponentOwner->GetActorRotation();


	FVector OwnerForwardVector = ComponentOwner->GetActorForwardVector();
	
	FVector Start = OwnerLocation;
	FVector End = OwnerLocation + OwnerForwardVector * 40;

	

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ComponentOwner);

	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);



}

void UPlayerMovementComponent::TrySetMoveClimb()
{
	FHitResult HitResult;
	if (ClimbingLineTrace(HitResult))
	{
		FRotator Normal_Rot = FRotationMatrix::MakeFromX(HitResult.Normal).Rotator();

		FRotator Player_Rot = GetOwner()->GetActorRotation();

		Player_Rot.Yaw = Normal_Rot.Yaw + 180.f;

		Player_Rot.Pitch = -Normal_Rot.Pitch;


		bOrientRotationToMovement = false;

		GetOwner()->SetActorRotation(FRotator(Player_Rot));

		SetMovementClimb();
	}
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


