// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelBeforeExplosion.h"
#include "Misc/Utils.h"
#include "Actors/Effect/ParticleEffect_AlphaUpScaleUp.h"

void UAnimNotify_LynelBeforeExplosion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	const AActor* Monster = MeshComp->GetOwner();
	UWorld* World = Monster->GetWorld();
	AParticleEffect_AlphaUpScaleUp* ParticleEffect = World->SpawnActorDeferred<AParticleEffect_AlphaUpScaleUp>(AParticleEffect_AlphaUpScaleUp::StaticClass(),
		FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	FTransform NewTransform;
	ParticleEffect->SetData(ParticleEffectName::Lynel_BeforeExplosion);

	const FVector Location = Monster->GetActorLocation();
	NewTransform.SetLocation(Location);

	ParticleEffect->FinishSpawning(NewTransform);
}
