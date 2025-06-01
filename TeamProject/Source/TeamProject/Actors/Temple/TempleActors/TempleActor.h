// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempleActor.generated.h"

#define FLOATINGACTOR_FORCE						800.f
#define FLOATINGACTOR_MASS_KG			        1000.f
#define FLOATINGACTOR_LINEAR_DAMPING			1.2f

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
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

	USphereComponent* GetCollisionComponent() { return CollisionComponent; }

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
};
