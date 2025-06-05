// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile/Projectile.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Projectile_Arrow.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
struct FNiagaraEffectTableRow;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AProjectile_Arrow : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectile_Arrow();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FName GetProjectileName() override;

	virtual void SetData(const FName& ProjectileName, FName ProfileName) override;




public:
	bool GetIsFire() { return bIsFire; }

protected:

	bool bIsFire = false;

};
