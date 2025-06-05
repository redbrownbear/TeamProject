// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_MonsterPlayerAlert.h"
#include "AnimNotify_MonsterPlayerAlert.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Monster/PawnMonster.h"
#include "Misc/Utils.h"

void UAnimNotify_MonsterPlayerAlert::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APawn* Pawn = Cast<APawn>(MeshComp->GetOwner()))
	{
		if (Pawn->IsA<APawnMonster>())
		{
			UWorld* World = MeshComp->GetWorld();

			AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
				FTransform::Identity, Pawn, Pawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Projectile->SetData(ProjectileName::Monster_PlayerAlert, CollisionProfileName::ToMonster);

			FVector Location = Pawn->GetActorLocation();
			NewTransform.SetLocation(Location);

			Projectile->FinishSpawning(NewTransform);
		}
	}
}
