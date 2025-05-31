// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/KogaStone.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Data/ProjectileTableRow.h"

// Sets default values
AKogaStone::AKogaStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKogaStone::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovementComponent->Velocity = FVector::Zero();
}

// Called every frame
void AKogaStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaitTime += DeltaTime;

	if (!bFall)
	{
		if (WaitTime > KOGASTONE_WAIT_TIME_MAX)
		{
			if (!bVelocitySet)
			{
				bVelocitySet = true;
				SetVelocity();
			}
		}
		else
		{
			if (eKind == EKogaStoneKind::SECOND)
			{

				if (AActor* CenterActor = GetOwner())
				{
					// X축 공전: Pitch 축 기준으로 회전 = 언리얼 좌표계 기준으로 YZ 평면에서 회전
					OrbitAngle += 360.f * DeltaTime;

					// 도 → 라디안
					float Radians = FMath::DegreesToRadians(OrbitAngle);

					// 기준 위치
					FVector Center = CenterActor->GetActorLocation();

					// X축을 기준으로 회전하면, YZ 평면 상에서 원을 그림
					float Y = FMath::Cos(Radians) * OrbitRadius;
					float Z = FMath::Sin(Radians) * OrbitRadius;

					FVector NewLocation = Center + FVector(0.f, Y, Z);
					SetActorLocation(NewLocation);
				}
			}
		}
	}
}

void AKogaStone::SetFallTrue()
{
	bFall = true;
	ProjectileMovementComponent->ProjectileGravityScale = 1.f;
	ProjectileMovementComponent->Velocity = FVector::Zero();
}

void AKogaStone::SetVelocity()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (AActor* Player = PC->GetPawn())
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				const FVector ProjectileLocation = GetActorLocation();

				FVector Direction = PlayerLocation - ProjectileLocation;
				Direction.Normalize();

				ProjectileMovementComponent->Velocity = Direction * ProjectileTableRow->MaxSpeed;
			}
		}
	}
}


