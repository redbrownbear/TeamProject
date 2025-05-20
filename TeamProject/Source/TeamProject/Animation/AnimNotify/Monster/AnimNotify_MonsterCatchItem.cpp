// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_MonsterCatchItem.h"

#include "Actors/Monster/Monster.h"
#include "Actors/Projectile/Projectile.h"

#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"


void UAnimNotify_MonsterCatchItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::Monster_CatchItem, CollisionProfileName::ToItem);

		FVector Location = Monster->GetActorLocation();
		const FVector MonsterLook = Monster->GetActorForwardVector();
		Location += MONSTER_ATTACK_PROJECTILE_OFFSET * MonsterLook;

		NewTransform.SetLocation(Location);
		NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		Projectile->FinishSpawning(NewTransform);
	}
}
