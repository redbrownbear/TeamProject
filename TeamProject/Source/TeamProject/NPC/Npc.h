#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NpcController.h"

#include "Components/SphereComponent.h"
//#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"
//#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "NPCFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Npc.generated.h"

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
	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMonsterStatusComponent> StatusComponent;*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAdvancedFloatingPawnMovement> MovementComponent;*/
};
