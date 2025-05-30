// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

#define FLOATINGACTOR_FORCE						800.f
#define FLOATINGACTOR_MASS_KG			        1000.f
#define FLOATINGACTOR_LINEAR_DAMPING			1.2f

class UBoxComponent;
class USplineComponent;
class AFlowSurface;
struct FFloatingActorTableRow;

UCLASS()
class TEAMPROJECT_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

	UBoxComponent* GetCollisionComponent() { return CollisionComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
	FDataTableRowHandle DataTableRowHandle;

	FFloatingActorTableRow* FloatingActorData;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial; 

	UPROPERTY(EditInstanceOnly, Category = "Flow")
	TObjectPtr<AFlowSurface> FlowSurface = nullptr;

protected:
	float DistanceAlongSpline = 0.f;

	UPROPERTY(EditAnywhere, Category = "Flow")
	float FloatingSpeed = 0.f;
};
