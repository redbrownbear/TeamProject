// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void SetAttackBox();

	void SetCanAttack();

	void SetCanMove();
		
protected:

	bool bCanAttack = true;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> Arr_Sword_Attack_MTG;

	UPROPERTY()
	int32 CurrentComboIndex = 0;

	UPROPERTY()
	int32 MaxComboIndex = 0;



};
