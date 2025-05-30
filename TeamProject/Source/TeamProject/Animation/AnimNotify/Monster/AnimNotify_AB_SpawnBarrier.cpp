// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_AB_SpawnBarrier.h"
#include "Actors/Object/Barrier.h"
#include "Actors/Monster/CharacterMonster.h"

void UAnimNotify_AB_SpawnBarrier::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		ABarrier* Barrier = World->SpawnActorDeferred<ABarrier>(ABarrier::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		const FVector Location = Monster->GetActorLocation();
		const FVector FowardVector = Monster->GetActorForwardVector();
		FVector SpawnLocation = Location + FowardVector * 100.f;
		NewTransform.SetLocation(SpawnLocation);
		Barrier->FinishSpawning(NewTransform);
	}
}
