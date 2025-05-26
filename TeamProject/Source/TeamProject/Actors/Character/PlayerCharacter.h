// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Actors/Weapon/WeaponSword.h"
#include "Actors/Weapon/WeaponBow.h"
#include "Actors/Weapon/WeaponArrow.h"
#include "Components/WeaponChildActorComponent/WeaponChildActorComponent.h"
#include "Components/StatusComponent/PlayerStatusComponent/PlayerStatusComponent.h"
#include "Components/Character/WeaponManagerComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"
#include "Misc/Utils.h"
#include "PlayerCharacter.generated.h"



UCLASS()
class TEAMPROJECT_API APlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
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

	
	

public:
	UWeaponChildActorComponent* GetSword() { return WeaponManagerComponent->GetSword(); }
	UWeaponChildActorComponent* GetShield() { return WeaponManagerComponent->GetShield(); }
	UWeaponChildActorComponent* GetBow() { return WeaponManagerComponent->GetBow(); }
	UPlayerStatusComponent* GetPlayerStatusComponent() { return StatusComponent; }
	UWeaponManagerComponent* GetWeaponManagerComponent() { return WeaponManagerComponent; }
	USpringArmComponent* GetSpringArm() { return SpringArm; }


	UFUNCTION()
	void TimelineProgress(float Value);


	void ZoomIn();
	void ZoomOut();

protected:

	/*UPROPERTY(EditAnywhere, Category="Weapon")
	TObjectPtr<UWeaponComponent> SwordComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UWeaponManagerComponent> WeaponManagerComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UTimelineComponent> ZoomTimeline;

	FOnTimelineFloat InterpFunction{};



	UPROPERTY(EditAnywhere, Category = "Status")
	TObjectPtr<UPlayerStatusComponent> StatusComponent;

	bool bZoomedIn;


	UPROPERTY(EditAnywhere, Category = "Zoom")
	UCurveFloat* ZoomCurve;




public:
	virtual uint8 GetGenericTeamId() { return FGenericTeamId(PLAYER_GENERIC_TEAM_ID); }
};
