// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
// #include "Components/AttackComponent/WeaponComponent.h"
#include "Actors/Weapon/WeaponSword.h"
#include "Actors/Weapon/WeaponBow.h"
#include "Components/WeaponChildActorComponent/WeaponChildActorComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TEAMPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 에디터에서 엑터의 변경마다 호출되는 함수
	virtual void OnConstruction(const FTransform& Transform);

	
	// 칼 휘두르는 몽타주 실행

	void Play_Sword_Attack();



public:
	AWeaponSword* GetSword() { return Cast<AWeaponSword>(Sword->GetChildActor()); }



protected:

	/*UPROPERTY(EditAnywhere, Category="Weapon")
	TObjectPtr<UWeaponComponent> SwordComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Lower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Upper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UWeaponChildActorComponent> Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UWeaponChildActorComponent> Sword;

protected:
	
};
