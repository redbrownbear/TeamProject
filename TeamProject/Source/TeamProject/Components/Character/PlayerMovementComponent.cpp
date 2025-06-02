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
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset{
			TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Glide/Equip_Float_Off_Montage.Equip_Float_Off_Montage'")
		};

		if (Asset.Object)
		{
			GlideUnEquip = Asset.Object;
		}
	}
	MaxWalkSpeedCrouched = PLAYER_MOVE_CROUCH;
	MaxWalkSpeed = PLAYER_MOVE_NML;

}

void UPlayerMovementComponent::BeginPlay()
{

	Super::BeginPlay();
	
	GetNavAgentPropertiesRef().bCanCrouch = true;

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

	FVector UpEnd = Start + CharacterUpVector * PLAYER_CAPSULE_HALF_HEIGHT * 0.5;

	FVector CharacterForwardVector = OwnerActor->GetActorForwardVector();
	FVector ForwardEnd = UpEnd + CharacterForwardVector * PLAYER_CAPSULE_RADIUS * 3;
	FVector DownEnd = ForwardEnd - CharacterUpVector * PLAYER_CAPSULE_HALF_HEIGHT * 2;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnerActor);

	DrawDebugLine(GetWorld(), ForwardEnd, DownEnd, FColor::Red, false, 2.f);
	
	
	bool CanStand = !GetWorld()->LineTraceSingleByChannel(
		HitResult,
		UpEnd,
		ForwardEnd,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);
	//캐릭터 머리가 벽보다 높을 때,
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
	

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());
	if (!_bool)
	{
		Player_C->JumpCurrentCount = 0;
	}
	if (bIsGliding)
	{
		SetGlideMode(false);

		UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

	}
	MovementMode = _bool ? MOVE_Flying : MOVE_Walking;

	

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

void UPlayerMovementComponent::SetGlideMode(bool _bool)
{
	if (_bool)
	{
		if (!CanGlide())
		{
			return;
		}

	}
	
  	bIsGliding = _bool;

	GravityScale = _bool ? 0.07: 1.f;

	AirControl = _bool ? 0.f : 0.f;

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	FHitResult HitResult;

	Player_C->OnLanded(HitResult);
	
	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

	AnimInst->bIsGliding = _bool;

	if (_bool)
	{
		if (Velocity.Size2D() < PLAYER_GLIDE_MIN_SPEED)
		{
			float Origin_Z = Velocity.Z;


			FRotator Player_Rot = GetOwner()->GetActorRotation();

			FVector DirectionXY = Player_Rot.Vector();

			Velocity = DirectionXY * PLAYER_GLIDE_MIN_SPEED;

			Velocity.Z = 0 < Origin_Z ? 0 : Origin_Z;
		}
		UWeaponManagerComponent* WeaponManagerComponent = Player_C->GetWeaponManagerComponent();
		if (_bool)
		{
			AnimInst->Montage_Play(WeaponManagerComponent->GetGliderEquipMontage());
		}
		else
		{
			WeaponManagerComponent->GetGlider()->SetVisibility(false);
			AnimInst->Montage_Play(WeaponManagerComponent->GetGliderUnEquipMontage());
		}
	}
}

void UPlayerMovementComponent::GlidingMove(FVector2D ActionValue)
{

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	Player_C->GetVelocity();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if(ActionValue.Y != 0)
	{
		FRotator NewRotation = Player_C->GetActorRotation();
		NewRotation.Yaw += ActionValue.Y * PLAYER_GLIDE_ROTATE_SPEED * DeltaTime;
		Player_C->SetActorRotation(NewRotation);

		float ZSpeed = Velocity.Z;
		Velocity.Z = 0.f;
		FVector NewDirection = Player_C->GetActorForwardVector();
		float Speed = Velocity.Size();
		Velocity = NewDirection * Speed;
		Velocity.Z = ZSpeed;
	}

	if (ActionValue.X != 0)
	{
		float Speed = Velocity.Size2D();
		float NewSpeed = FMath::Clamp(Speed + DeltaTime* ActionValue.X * PLAYER_GLIDE_MODIFY_SPEED, PLAYER_GLIDE_MIN_SPEED,PLAYER_GLIDE_MAX_SPEED);

		if (Speed != NewSpeed)
		{
			float Origin_Z = Velocity.Z;

			FVector DirectionXY = FVector(Velocity.X, Velocity.Y, 0.f).GetSafeNormal();

			Velocity = DirectionXY * NewSpeed;
			Velocity.Z = Origin_Z;
		}


	}


}

bool UPlayerMovementComponent::CanGlide()
{
	AActor* Owner_C = GetOwner();
	
	FVector Start = Owner_C->GetActorLocation();
	FVector C_Up_Vector = Owner_C->GetActorUpVector();
	FVector End = Start - C_Up_Vector * PLAYER_CAPSULE_HALF_HEIGHT * 5;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Owner_C);

	FHitResult HitResult;

	return !GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);

}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


