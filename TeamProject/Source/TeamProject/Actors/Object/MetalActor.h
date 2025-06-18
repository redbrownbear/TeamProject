// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetalActor.generated.h"


class USphereComponent;
class UPhysicalMaterial;
class UMetalComponent;
class URewindComponent;
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

	virtual void OnConstruction(const FTransform& Transform);
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;

public:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void SetData(const FName& MetalActorName);

	//USphereComponent* GetCollisionComponent() { return CollisionComponent; }

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



public:
	virtual void ThisIsMetal();
	void ChangeNomalColor();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMetalComponent> MetalComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URewindComponent> RewindComponent;

	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetalActorTableRow")
	FDataTableRowHandle DataTableRowHandle;

	FMetalActorTableRow* MetalActorData;
	const UDataTable* MetalActorDataTable;

};
