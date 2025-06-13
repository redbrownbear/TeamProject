// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelAttack.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Effect/NiagaraEffect.h"
#include "Misc/Utils.h"


void UAnimNotify_LynelAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		{
			AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
				FTransform::Identity, Monster, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_LynelAttack, CollisionProfileName::ToPlayer);

			//const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
			//const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Weapon_Right);
			FVector Location = Monster->GetActorLocation();
			Location += Monster->GetActorForwardVector() * 100.f;
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
		{
			ANiagaraEffect* NiagaraEffect = World->SpawnActorDeferred<ANiagaraEffect>(ANiagaraEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			NiagaraEffect->SetData(NiagaraEffectName::Lynel_Attack);

			FVector Location = Monster->GetActorLocation();
			FRotator Rotation = Monster->GetActorRotation();
			Location += Monster->GetActorForwardVector() * 100.f;
			NewTransform.SetLocation(Location);
			NewTransform.SetRotation(Rotation.Quaternion());
			NiagaraEffect->FinishSpawning(NewTransform);
		}
	}


}
