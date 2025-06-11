// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxGrab.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile.h"

void UAnimNotify_HinoxGrab::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_HinoxGrab::Notify"));

		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, Monster, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::Monster_HinoxGrab, CollisionProfileName::ToPlayer);

		const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
		const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Weapon_Right);
		NewTransform.SetLocation(Location);

		Projectile->FinishSpawning(NewTransform);
	}
}
