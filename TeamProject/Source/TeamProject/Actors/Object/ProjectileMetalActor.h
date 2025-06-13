// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Object/MetalActor.h"
#include "ProjectileMetalActor.generated.h"

class UNiagaraComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class TEAMPROJECT_API AProjectileMetalActor : public AMetalActor
{
	GENERATED_BODY()

protected:
	AProjectileMetalActor();
	virtual void BeginPlay() override;


	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FVector GetVelocity() const;
	FName GetProjectileName() const;
	float GetDamage();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraEffectComponent;
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> ParticleEffectComponent;

public:
	void SetGrabbed(bool bFlag);
	void SetProjectileGravity(bool bFlag);
	void SetStaticMeshVisibility(bool bFlag);
	void SetNiagaraVisibility(bool bFlag);


private:
	bool bFall = false;
public:
	void SetFallTrue();
private:
	float WaitTime = 0.f;
	bool bVelocitySet = false;
	void SetVelocity();
};
