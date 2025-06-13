// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetalActor.generated.h"


class USphereComponent;
class UPhysicalMaterial;
struct FMetalActorTableRow;

UCLASS()
class TEAMPROJECT_API AMetalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void SetData(const FName& MetalActorName);

	//USphereComponent* GetCollisionComponent() { return CollisionComponent; }

public:
	virtual void ThisIsMetal();
	void ChangeNomalColor();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialInterface;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
	FDataTableRowHandle DataTableRowHandle;

	FMetalActorTableRow* MetalActorData;
	const UDataTable* MetalActorDataTable;

};
