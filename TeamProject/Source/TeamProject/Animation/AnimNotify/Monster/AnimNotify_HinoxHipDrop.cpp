// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxHipDrop.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Effect/ParticleEffect.h"

void UAnimNotify_HinoxHipDrop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_HinoxHipDrop::Notify"));


		UWorld* World = MeshComp->GetWorld();
		{
			AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
				FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_HinoxHipDrop, CollisionProfileName::ToPlayer);

			//const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
			//const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Toe_L);
			const FVector Location = Monster->GetActorLocation();
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
		///////////////////////////////////////////

		{
			AParticleEffect* ParticleEffect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			ParticleEffect->SetData(ParticleEffectName::Hinox_HipdropShockWave);

			const FVector Location = Monster->GetActorLocation();
			NewTransform.SetLocation(Location);

			ParticleEffect->FinishSpawning(NewTransform);
		}


	}
}
