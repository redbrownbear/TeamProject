// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


USTRUCT()
struct TEAMPROJECT_API FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	TSubclassOf<UAnimInstance> AnimClass;
};


UCLASS()
class TEAMPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	

public:

	AWeaponBase();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void LeftClickAction();
	virtual void RightClickAction();
	UAnimMontage* GetUnEquipMontage() { return UnEquipMontage; }
	UAnimMontage* GetEquipMontage() { return EquipMontage; }


protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere)
	UAnimMontage* UnEquipMontage;

protected:
	friend class UItemAction_Weapon;
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/TeamProject.WeaponTableRow"))
	FDataTableRowHandle DataTableRowHandle;
};
