// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer)
{

	BrakingDecelerationFlying = 10000.f;
	AirControl = 0.f;
	MaxFlySpeed = PLAYER_CLIMB_SPEED;
	
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset{ 
			TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Move/Climb_Up_Land.Climb_Up_Land'") 
		};

		if (Asset.Object)
		{
			LandUpMontage = Asset.Object;
		}
	}



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

	return Returnbool;
}

bool UPlayerMovementComponent::TrySetMoveClimb(FVector2D ActionValue)
{
	FHitResult HitResult;

	if (ActionValue.X == 1)
	{
		CanClimbUpLand();
	}
	else if (ActionValue.X == -1)
	{
		CanClimbDownLand();
	}

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
		Player_C->GetController()->SetControlRotation(FRotator(Player_Rot));
		Player_C->SetActorLocation(NewLocation);

		return true;
	}
	

	SetClimbMode(false);

	return false;
}

bool UPlayerMovementComponent::CanClimbUpLand()
{

	AActor* OwnerActor = GetOwner();

	FHitResult HitResult;

	FVector Start = OwnerActor->GetActorLocation();

	FVector CharacterUpVector = OwnerActor->GetActorUpVector();

	FVector UpEnd = Start + CharacterUpVector * PLAYER_CAPSULE_HALF_HEIGHT * 2;

	FVector CharacterForwardVector = OwnerActor->GetActorForwardVector();
	FVector ForwardEnd = UpEnd + CharacterForwardVector * PLAYER_CAPSULE_RADIUS * 3;
	FVector DownEnd = ForwardEnd - CharacterUpVector * PLAYER_CAPSULE_HALF_HEIGHT * 2;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnerActor);

	
	
	bool CanStand = !GetWorld()->LineTraceSingleByChannel(
		HitResult,
		UpEnd,
		ForwardEnd,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);

	if (CanStand)
	{


		CanStand = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			ForwardEnd,
			DownEnd,
			ECollisionChannel::ECC_Visibility,
			TraceParams
		);

		if (CanStand)
		{
			UE_LOG(LogTemp, Warning, TEXT("CanStand"));
			APlayerCharacter* Player_C = Cast<APlayerCharacter>(OwnerActor);


			Climb_State = EClimb_State::Land;

			Player_C->GetMesh()->GetAnimInstance()->Montage_Play(LandUpMontage);
			

			return true;


		}
	}
	return false;
}

bool UPlayerMovementComponent::CanClimbDownLand()
{
	AActor* OwnerActor = GetOwner();

	FHitResult HitResult;

	FVector Start = OwnerActor->GetActorLocation();

	FVector CharacterUpVector = OwnerActor->GetActorUpVector();

	FVector End = Start - CharacterUpVector * (PLAYER_CAPSULE_HALF_HEIGHT* 1.5);


	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnerActor);

	bool CanStand = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);
	if (CanStand)
	{
		SetClimbMode(false);
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

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

	AnimInst->bIsClimingLand = false;

	USpringArmComponent* SpringArm = Player_C->GetSpringArm();

	SpringArm->bUsePawnControlRotation = !_bool;

	SpringArm->bEnableCameraRotationLag = _bool;

	Climb_State = EClimb_State::Climb;

}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


