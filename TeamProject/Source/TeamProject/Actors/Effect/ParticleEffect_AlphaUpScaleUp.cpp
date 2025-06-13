// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effect/ParticleEffect_AlphaUpScaleUp.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

void AParticleEffect_AlphaUpScaleUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FLinearColor Color;
	//ParticleEffectComponent->GetColorParameter(TEXT("Temp"), Color);
	//Color.A += 1 * DeltaTime;
	//Color.A = FMath::Clamp(Color.A, 0.f, 0.8f);
	//ParticleEffectComponent->SetColorParameter(TEXT("Temp"), Color);

	const FVector CurrentActorScale = GetActorScale3D();
	const FVector NewActorScale = CurrentActorScale + CurrentActorScale * 3.f * DeltaTime;
	SetActorScale3D(NewActorScale);
}
