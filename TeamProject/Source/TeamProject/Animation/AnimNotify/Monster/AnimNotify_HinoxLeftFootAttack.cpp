// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxLeftFootAttack.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile.h"
#include "Misc/Utils.h"

void UAnimNotify_HinoxLeftFootAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::Monster_HinoxAttack, CollisionProfileName::ToPlayer);

		const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
		const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Toe_L);
		NewTransform.SetLocation(Location);

		Projectile->FinishSpawning(NewTransform);
	}

}
