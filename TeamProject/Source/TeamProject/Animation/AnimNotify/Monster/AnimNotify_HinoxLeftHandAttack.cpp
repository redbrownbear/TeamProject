// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxLeftHandAttack.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Effect/ParticleEffect.h"

#include "Misc/Utils.h"

void UAnimNotify_HinoxLeftHandAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_HinoxLeftHandAttack::Notify"));

		UWorld* World = MeshComp->GetWorld();
		{
			AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
				FTransform::Identity, Monster, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_HinoxAttack, CollisionProfileName::ToPlayer);

			const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
			const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Weapon_Left);
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
		{
			AParticleEffect* ParticleEffect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			ParticleEffect->SetData(ParticleEffectName::Hinox_AttackShockWave);

			const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
			const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Weapon_Left);
			NewTransform.SetLocation(Location);

			ParticleEffect->FinishSpawning(NewTransform);
		}
	}
}
