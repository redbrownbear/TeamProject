// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dog.generated.h"

class USphereComponent;
class UAdvancedFloatingPawnMovement;
class UNpcFSMComponent;

UCLASS()
class TEAMPROJECT_API ADog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> BodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UAdvancedFloatingPawnMovement> MovementComponent;

public:
	UNpcFSMComponent* GetFSMComponent() const;

public:
	//void PlayMontage(EDogMontage _InEnum, bool bIsLoop = false);
	//bool IsMontage(EDogMontage _InEnum);
	//bool IsPlayingMontage(EDogMontage _InEnum);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;

private:
	// ��ȣ�ۿ� ���� ����
	UPROPERTY()
	bool bPlayerInRange = false;

	UPROPERTY() 
	bool bIsConfirm = false;

protected:
	// ��ȣ�ۿ� 
	UFUNCTION()
	void OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
