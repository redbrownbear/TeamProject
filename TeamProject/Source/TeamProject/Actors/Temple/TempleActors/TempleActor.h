// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempleActor.generated.h"

class USphereComponent;
class UPhysicalMaterial;
struct FTempleActorTableRow;

UCLASS()
class TEAMPROJECT_API ATempleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATempleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform);

	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

	void Initialize(class ATempleActorSpawner* InOwner) { SpawnerOwner = InOwner; }
	void DeactivateActor() { bIsActive = false; }
	void ActivateActor() { bIsActive = true; }
	bool IsActive() const { return bIsActive; }

	USphereComponent* GetCollisionComponent() { return CollisionComponent; }
	ATempleActorSpawner* GetTempleActorSpawner() { return SpawnerOwner; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
	FDataTableRowHandle DataTableRowHandle;

	FTempleActorTableRow* TempleActorData;

	UPROPERTY()
	TObjectPtr<ATempleActorSpawner> SpawnerOwner;

private:
	UPROPERTY()
	bool bIsActive = false;

};
