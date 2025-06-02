// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/KogaStone.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Data/ProjectileTableRow.h"

#include "Actors/Monster/CharacterMonster.h"

#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"

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

		if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(Owner))
		{
			if (UAssasinBossFSMComponent* FSMComponent = Cast<UAssasinBossFSMComponent>(CharacterMonster->GetFSMComponent()))
			{
				if (EMonsterState::Damage == FSMComponent->GetMonsterState())
				{
					UE_LOG(LogTemp, Warning, TEXT("KogaStone // SetFallTrue"));
					SetFallTrue();
					return;
				}
			}
		}


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

				if (AActor* CenterActor = GetInstigator())
				{
					OrbitAngle += KOGASTONE_ROTATE_SPEED * DeltaTime;
					float TotalAngle = OrbitAngle + InitialAngleOffset;
					float Radians = FMath::DegreesToRadians(TotalAngle);

					// 회전 평면을 Instigator 기준으로 맞춤
					FVector Forward = CenterActor->GetActorForwardVector(); // X
					FVector Right = CenterActor->GetActorRightVector();     // Y
					FVector Up = CenterActor->GetActorUpVector();           // Z

					// 원형 궤도: Right(=Y) & Up(=Z) 기준으로 회전
					FVector Offset = FMath::Cos(Radians) * Right * OrbitRadius + FMath::Sin(Radians) * Up * OrbitRadius;

					FVector Center = CenterActor->GetActorLocation();
					FVector NewLocation = Center + Offset;
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


