// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelHorn.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile_FollowOwner.h"

void UAnimNotify_LynelHorn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		{
			AProjectile_FollowOwner* Projectile = World->SpawnActorDeferred<AProjectile_FollowOwner>(AProjectile_FollowOwner::StaticClass(),
				FTransform::Identity, Monster, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_LynelHorn, CollisionProfileName::ToPlayer);

			FVector Location = Monster->GetActorLocation();
			Location += Monster->GetActorForwardVector() * 100.f;
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
	}
}
