// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

class UBoxComponent;
class USplineComponent;
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
	//UPROPERTY(VisibleAnywhere)
	//TArray<TObjectPtr<UBoxComponent>> CollisionComponent_Array;
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<USplineComponent> SplineComponent;
	//TArray<float> fDistanceAlongSpline_Array;

protected:
	bool bMove = false;
};
