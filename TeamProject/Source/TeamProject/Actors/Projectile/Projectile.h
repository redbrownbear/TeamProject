// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

struct FProjectileTableRow;

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
	UPROPERTY()
	class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY()
	class UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "ProjectileTableRow"))
	FDataTableRowHandle DataTableRowHandle;
	const UDataTable* ProjectileDataTable;
	const FProjectileTableRow* ProjectileTableRow;

protected:
	ECollisionChannel CollisionChannel;

public:
	FVector GetVelocity();
	FName GetProjectileName();


};
