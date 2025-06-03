// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_AL_AttackBig.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Misc/Utils.h"

void UAnimNotify_AL_AttackBig::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		const FVector MonsterForwardVector = Monster->GetActorForwardVector();
		NewTransform.SetRotation(MonsterForwardVector.Rotation().Quaternion());


		Projectile->SetData(ProjectileName::Monster_AL_AttackBig, CollisionProfileName::ToPlayer);

		const FVector Location = Monster->GetActorLocation();
		NewTransform.SetLocation(Location);



		Projectile->FinishSpawning(NewTransform);
	}
}
