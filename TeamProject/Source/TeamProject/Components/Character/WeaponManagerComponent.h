// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WeaponChildActorComponent/WeaponChildActorComponent.h"
#include "Misc/Utils.h"
#include "Containers/Queue.h"
#include "WeaponManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:
	UWeaponChildActorComponent* GetSword() { return Sword; }
	UWeaponChildActorComponent* GetShield() { return Shield; }
	UWeaponChildActorComponent* GetBow() { return Bow; }


	EWeapon_Type GetNextWeaponType() { return NextWeapon; }
	void SetNextWeaponType(EWeapon_Type _Type) { NextWeapon = _Type; }
	EEquip_State GetEquipState() { return Equip_State; }
	void SetEquipState(EEquip_State _State) { Equip_State = _State; }

	void SetRightClick(bool _RightClick) { bRightClick = _RightClick; }
	bool GetRightClick() { return bRightClick; }

	void SetCanShot(bool _CanShot) { bCanShot = _CanShot; }
	bool GetCanShot() { return bCanShot; }




	EWeapon_Type GetUnEquipWeaponType() {
		EWeapon_Type _Type;
		UnEquipWeapons.Dequeue(_Type);
		return _Type;
	}



	void TryEquipWeapon();
	void EquipWeapon(UAnimMontage* Montage, bool bInterrupted);


	void LeftClickAction();
	void RightClickAction();




protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UWeaponChildActorComponent> Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UWeaponChildActorComponent> Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UWeaponChildActorComponent> Bow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UWeaponChildActorComponent> Arrow;

	UPROPERTY()
	EEquip_State Equip_State;
	UPROPERTY()
	EWeapon_Type NextWeapon;

	TQueue<EWeapon_Type> UnEquipWeapons;

	UPROPERTY()
	UAnimMontage* UnEquip_Sword_Shield;


	bool bRightClick;
	bool bCanShot;
};
