// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Utils.h"
#include "Data/ItemDataRow.h"
#include "WorldWeapon.generated.h"

struct FItemData;
class UShapeComponent;
class UPhysicalMaterial;
class UAIPerceptionStimuliSourceComponent;
class IMonsterInterface;
class ALynel;

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
	virtual void SetDataWithData(const FItemData& InItemData);

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
	UFUNCTION()
	void OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION()
	//virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	void PickUpItem();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;
	

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "ItemData"))
	FDataTableRowHandle DataTableRowHandle;
	
	const UDataTable* ItemDataTable;
	const FItemData* ItemTableRow;

	UPROPERTY()
	FItemData ItemDataCopy;

public:
	FName GetWorldWeaponName() const;
	EWeaponKind GetWorldWeaponKind() const;
	void AddForce(FVector _Direction, float Force);
	void AttachToMonster(IMonsterInterface* Monster, FName SocketName);
	void DetachFromMonster();

protected:
	bool bIsCatched = false;
public:
	bool GetCatched() const { return bIsCatched; }
	float GetDamage() const;

};
