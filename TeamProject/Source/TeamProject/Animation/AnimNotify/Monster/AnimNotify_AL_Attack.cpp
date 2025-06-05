// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_AL_Attack.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Effect/NiagaraEffect.h"
#include "Misc/Utils.h"

void UAnimNotify_AL_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		{
			AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
				FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_AL_Attack, CollisionProfileName::ToPlayer);

			//const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
			//const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Toe_L);
			const FVector Location = Monster->GetActorLocation();
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
		{
			ANiagaraEffect* NiagaraEffect = World->SpawnActorDeferred<ANiagaraEffect>(ANiagaraEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			NiagaraEffect->SetData(NiagaraEffectName::AL_Attack);

			FVector Location = Monster->GetActorLocation();
			Location += Monster->GetActorForwardVector() * 100.f;
			NewTransform.SetLocation(Location);

			NiagaraEffect->FinishSpawning(NewTransform);
		}

	}
}
