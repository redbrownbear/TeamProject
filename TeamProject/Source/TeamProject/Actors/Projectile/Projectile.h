// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

struct FProjectileTableRow;
class UNiagaraComponent;
class UParticleSystemComponent;
class UNiagaraSystem;
class UStaticMeshComponent;
class UMyProjectileMovementComponent;
class URewindComponent;

UCLASS()
class TEAMPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();
	virtual void SetData(const FName& ProjectileName, FName ProfileName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UShapeComponent> CollisionComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY()
	TObjectPtr<UMyProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY()
	TObjectPtr<URewindComponent> RewindComponent;





protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "ProjectileTableRow"))
	FDataTableRowHandle DataTableRowHandle;
	const UDataTable* ProjectileDataTable;
	const FProjectileTableRow* ProjectileTableRow;

protected:
	ECollisionChannel CollisionChannel;
	bool bGetDamageFromWeapon = false;
public:
	FVector GetVelocity();
	virtual FName GetProjectileName();
	float GetDamage();
	
protected:
	UPROPERTY()
	TArray<UNiagaraComponent*> Trails;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraEffectComponent;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ProjectileNiagaraEffectComponent;
	UPROPERTY()
	UNiagaraSystem* TrailEffectFX;
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> ParticleEffectComponent;

	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
public:
	void SetProjectileMovementActivate(bool bFlag);
	void SetGravityScale(float Scale);
	void SetStaticMeshVisibility(bool bFlag);
	void SetNiagaraVisibility(bool bFlag);
	void SetPhysicsEnabled(bool bFlag);

public:
	void SetColorScanned(float Value = 0.8f);
	void SetColorNormal();
};
