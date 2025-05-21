// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Utils.h"
#include "WorldWeapon.generated.h"

struct FItemData;
class UShapeComponent;
class UPhysicalMaterial;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class TEAMPROJECT_API AWorldWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldWeapon();

	// for spawn from codes
	virtual void SetDataWithName(const FName& WorldWeaponName);
	virtual void SetDataWithHandle(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform);
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;
	

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "ItemData"))
	FDataTableRowHandle DataTableRowHandle;
	const UDataTable* ItemDataTable;
	const FItemData* ItemTableRow;

public:
	FName GetWorldWeaponName();
	EWeaponKind GetWorldWeaponKind();
	void AddForce(FVector _Direction, float Force);

protected:
	bool bIsCatched = false;
public:
	bool GetCatched() { return bIsCatched; }
};
