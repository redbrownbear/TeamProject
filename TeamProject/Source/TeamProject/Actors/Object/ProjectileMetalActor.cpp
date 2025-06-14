// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/ProjectileMetalActor.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Effect/NiagaraEffect.h"
#include "Actors/Effect/ParticleEffect.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Monster/CharacterMonster.h"

#include "Misc/Utils.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"
#include "Components/SphereComponent.h"

AProjectileMetalActor::AProjectileMetalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = KOGASTONE_SPEED;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 15.f;

	RootComponent = CollisionComponent;
	//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	//CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void AProjectileMetalActor::BeginPlay()
{
	Super::BeginPlay();
	// We dont need Collision's Physcis in ProjectileMetalActor
	CollisionComponent->SetSimulatePhysics(false);
	//CollisionComponent->SetSphereRadius(110.f);
	CollisionComponent->SetEnableGravity(false);
	CollisionComponent->SetHiddenInGame(COLLISION_HIDDEN_IN_GAME);

	//UE_LOG(LogTemp, Warning, TEXT("Radius after set: %f"), CollisionComponent->GetUnscaledSphereRadius());
	//ProjectileMovementComponent->Activate();
	SetProjectileGravity(false);


	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetEnableGravity(false);
	
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AProjectileMetalActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(this)) { return; }

	if (APlayerCharacter* Player_C = Cast<APlayerCharacter>(OtherActor))
	{
		Player_C->Damaged(GetDamage());
	}
	else if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(OtherActor))
	{
		if (UWorld* World = GetWorld())
		{
			AParticleEffect* ParticleEffect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			ParticleEffect->SetData(ParticleEffectName::Hinox_AttackShockWave);

			const FVector Location = GetActorLocation();
			NewTransform.SetLocation(Location);

			ParticleEffect->FinishSpawning(NewTransform);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AProjectileMetalActor::OnBeginOverlap // Cant find World"));
		}
	}

	Destroy();
}


void AProjectileMetalActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UWorld* World = GetWorld())
	{
		AParticleEffect* ParticleEffect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
			FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		ParticleEffect->SetData(ParticleEffectName::Hinox_AttackShockWave);

		const FVector Location = GetActorLocation();
		NewTransform.SetLocation(Location);

		ParticleEffect->FinishSpawning(NewTransform);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AProjectileMetalActor::OnHit // Cant find World"));
	}
	Destroy();
}

FVector AProjectileMetalActor::GetVelocity() const
{
	const FVector Direction = ProjectileMovementComponent->Velocity;
	return Direction;
}

FName AProjectileMetalActor::GetProjectileName() const
{
	const FName Name = DataTableRowHandle.RowName;
	return Name;
}

float AProjectileMetalActor::GetDamage()
{
	return 1.f;
}

void AProjectileMetalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaitTime += DeltaTime;

	if (!bFall)
	{
		if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(Owner))
		{
			if (UAssasinBossFSMComponent* FSMComponent = Cast<UAssasinBossFSMComponent>(CharacterMonster->GetFSMComponent()))
			{
				if (EMonsterState::Damage == FSMComponent->GetCurrentState())
				{
					UE_LOG(LogTemp, Warning, TEXT("AProjectileMetalActor // SetFallTrue"));
					SetFallTrue();
					return;
				}
			}
		}

		if (WaitTime > KOGASTONE_WAIT_TIME_MAX)
		{
			if (ProjectileMovementComponent->IsActive())
			{
				if (!bVelocitySet)
				{
					bVelocitySet = true;
					SetVelocity();
				}
			}
		}
	}
}

void AProjectileMetalActor::SetGrabbed(bool bFlag)
{
	if (bFlag)
	{
		ProjectileMovementComponent->Deactivate();
	}
	else
	{
		ProjectileMovementComponent->Activate();
	}
	CollisionComponent->SetSimulatePhysics(bFlag);
}

void AProjectileMetalActor::SetProjectileGravity(bool bFlag)
{
	if (bFlag)
	{	
		ProjectileMovementComponent->ProjectileGravityScale = 1.f;
	}
	else
	{
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	}
	
}

void AProjectileMetalActor::SetStaticMeshVisibility(bool bFlag)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetVisibility(bFlag);
	}
}

void AProjectileMetalActor::SetNiagaraVisibility(bool bFlag)
{
	if (!NiagaraEffectComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AProjectileMetalActor::SetNiagaraVisibility // No NiagaraEffectComponent"))
		return;
	}

	if (bFlag)
	{
		NiagaraEffectComponent->Activate();
	}
	else
	{
		NiagaraEffectComponent->Deactivate();
	}
}

void AProjectileMetalActor::SetFallTrue()
{
	bFall = true;
	ProjectileMovementComponent->Velocity = FVector::Zero();
	SetProjectileGravity(true);
}

void AProjectileMetalActor::SetVelocity()
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

				ProjectileMovementComponent->Velocity = Direction * KOGASTONE_SPEED;
				SetActorRotation(Direction.Rotation());
			}
		}
	}
}

