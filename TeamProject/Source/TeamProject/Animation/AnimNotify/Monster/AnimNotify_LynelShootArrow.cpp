// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelShootArrow.h"
#include "Actors/Monster/Monster.h"
#include "Actors/Projectile/Projectile.h"

void UAnimNotify_LynelShootArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::Monster_LynelArrow, CollisionProfileName::ToPlayer);

		const USkeletalMeshComponent* Mesh = Monster->GetSkeletalMeshComponent();;
		const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Weapon_R);

		const FVector MonsterForwardVector = Monster->GetActorForwardVector();
		NewTransform.SetLocation(Location);
		NewTransform.SetRotation(MonsterForwardVector.Rotation().Quaternion());

		Projectile->FinishSpawning(NewTransform);
	}
}
