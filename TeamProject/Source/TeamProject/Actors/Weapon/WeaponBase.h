// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Utils.h"
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

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStaticMesh(UStaticMesh* InMesh);
public:
	virtual void LeftClickAction();
	virtual void RightClickAction();
	UAnimMontage* GetUnEquipMontage() { return UnEquipMontage; }
	UAnimMontage* GetEquipMontage() { return EquipMontage; }

	EWeapon_Type GetWeaponType() { return WeaponType; }


protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere)
	UAnimMontage* UnEquipMontage;

	UPROPERTY(VisibleAnywhere)
	UAnimMontage* UpperNmlMontage;


protected:
	friend class UItemAction_Weapon;
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/TeamProject.WeaponTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	EWeapon_Type WeaponType;
};
