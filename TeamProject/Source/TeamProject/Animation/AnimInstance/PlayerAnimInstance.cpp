// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Character/PlayerCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn) { return; }

	MovementComponent = Pawn->GetMovementComponent();


}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!MovementComponent)
	{
		return;
	}


	Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	bShoudMove = !FMath::IsNearlyZero(Speed);

	bIsCrouch = MovementComponent->IsCrouching();

	bIsFalling = MovementComponent->IsFalling();


}

void UPlayerAnimInstance::SetPitch()
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwningActor());
	AController* Controller = Player_C->GetController();
	FRotator ControllerRotation = Controller->GetControlRotation();
	ControllerRotation.Roll = 0.f;
	ControllerRotation.Yaw = 0.f;

	FVector NewForward = ControllerRotation.Vector().GetSafeNormal();

	
	FVector WorldUp = FVector::UpVector; // 기본은 (0, 0, 1)

	// 방향 벡터 기준으로 회전 행렬 생성
	FMatrix Basis = FRotationMatrix::MakeFromXZ(NewForward, WorldUp);

	// 기존 회전값
	FQuat OldQuat = ControllerRotation.Quaternion();

	// 새로운 회전 기준으로 변환
	FQuat NewQuat = Basis.ToQuat() * OldQuat;

	// 결과: 새 방향 기준으로 회전된 회전값
	Rotator = NewQuat.Rotator();


}
