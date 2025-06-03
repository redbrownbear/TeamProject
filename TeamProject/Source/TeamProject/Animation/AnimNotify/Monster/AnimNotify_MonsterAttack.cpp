// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_MonsterAttack.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"
#include "Misc/Utils.h"

void UAnimNotify_MonsterAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APawn* Pawn = Cast<APawn>(MeshComp->GetOwner()))
	{
		if (Pawn->IsA<ACharacterMonster>() || Pawn->IsA<APawnMonster>())
		{
			UWorld* World = MeshComp->GetWorld();

			AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
				FTransform::Identity, Pawn, Pawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_Attack, CollisionProfileName::ToPlayer);

			//const USkeletalMeshComponent* Mesh = Monster->GetMonsterMesh();
			//const FVector Location = Mesh->GetSocketLocation(Monster_SocketName::Toe_L);
			FVector Location = Pawn->GetActorLocation();
			const FVector ForwardVector = Pawn->GetActorForwardVector();
			Location += ForwardVector * 100.f;
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
	}
}
