// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_AB_SpawnBarrier.h"
#include "Actors/Object/Barrier.h"
#include "Actors/Monster/CharacterMonster.h"

void UAnimNotify_AB_SpawnBarrier::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UWorld* World = MeshComp->GetWorld())
		{
			ABarrier* Barrier = World->SpawnActorDeferred<ABarrier>(ABarrier::StaticClass(),
				FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			const FVector Location = Monster->GetActorLocation();

			const FVector PlayerLocation = World->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			

			FVector DirectionVector = PlayerLocation - Location;
			DirectionVector.Normalize();
			const FVector SpawnLocation = Location + DirectionVector * 100.f;
			const FRotator SpawnRotator = Monster->GetActorRotation();
			NewTransform.SetLocation(SpawnLocation);
			NewTransform.SetRotation(SpawnRotator.Quaternion());
			Barrier->FinishSpawning(NewTransform);
		}

	}
}
