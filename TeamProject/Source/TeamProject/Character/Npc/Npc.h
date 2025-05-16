#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Controller/Npc/NpcController.h"

#include "Npc.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
class UAdvancedFloatingPawnMovement;
class UNpcFSMComponent;

UCLASS()
class TEAMPROJECT_API ANpc : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANpc();

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
	TObjectPtr<UBoxComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> FaceMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> HairMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> NoseMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	TObjectPtr<UNpcFSMComponent> FSMComponent;

public:
	UNpcFSMComponent* GetFSMComponent() const;

protected:
	// 오버랩 상태
	UPROPERTY()
	bool bPlayerInRange = false;

	// 오버랩 함수
	UFUNCTION()
	void OnPlayerInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// 키 입력 함수
	void OnTalkKeyPressed();
};
