// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/Projectile_FollowOwner.h"
#include "Actors/Monster/CharacterMonster.h"

#include "Components/FSMComponent/Monster/LynelFSMComponent.h"

void AProjectile_FollowOwner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AActor* OwnerActor = GetOwner())
	{
		FVector OwnerLocation = OwnerActor->GetActorLocation();
		FVector OwnerForwardVector = OwnerActor->GetActorForwardVector();
		OwnerForwardVector.Normalize();
		OwnerLocation += OwnerForwardVector * 100.f;
		SetActorLocation(OwnerLocation);

		if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(OwnerActor))
		{
			if (UMonsterFSMComponent* MonsterFSMComponent = CharacterMonster->GetFSMComponent())
			{
				if (EMonsterState::HornAttack != MonsterFSMComponent->GetCurrentState())
				{
					Destroy();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AProjectile_FollowOwner::Tick // No Owner"));
		check(false);
	}
}
