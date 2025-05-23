// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelExplosion.h"
#include "Actors/Monster/Monster.h"
#include "Actors/Projectile/Projectile.h"

void UAnimNotify_LynelExplosion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::Monster_LynelExplosion, CollisionProfileName::ToPlayer);

		const FVector Location = Monster->GetActorLocation();
		NewTransform.SetLocation(Location);

		Projectile->FinishSpawning(NewTransform);
	}
}
