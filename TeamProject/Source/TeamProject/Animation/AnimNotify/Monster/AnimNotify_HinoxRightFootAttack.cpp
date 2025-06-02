// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxRightFootAttack.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Projectile/Projectile.h"

void UAnimNotify_HinoxRightFootAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_HinoxRightFootAttack::Notify"));

		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::Monster_HinoxAttack, CollisionProfileName::ToPlayer);

		const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
		const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Toe_R);
		NewTransform.SetLocation(Location);

		Projectile->FinishSpawning(NewTransform);
	}
}
