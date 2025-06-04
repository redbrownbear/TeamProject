// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureBox.generated.h"

class UBoxComponent;
class UPopupGetItem;
struct FItemData;

UCLASS()
class TEAMPROJECT_API ATreasureBox : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATreasureBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EmptyBox() 
	{
		bCanOpenBox = false;
		bCanTakeItem = false;
	}

public:
	UFUNCTION()
	void OpenTBox();

	UFUNCTION()
	void CloseUI();

	bool GetOpenBox() { return bCanOpenBox; }

protected:
	UFUNCTION()
	void OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UFUNCTION()
	void GetTreasure();

	void GetParticleEffect();

	void ShowItemByRowName(FName RowName);
	void ShowItemPopup(FName ItemRowName);
	void AddItemInventory();

public:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (RowType = "ParticleEffectTableRow"))
	FDataTableRowHandle ParticleEffectTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FDataTableRowHandle ItemRowHandle;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UDataTable* ItemDataTable;*/

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UPopupGetItem> PopupItemUI;

protected:
	UMaterialInterface* MaterialInterface;
	UMaterialInstanceDynamic* DynamicMaterialInstance;

private:
	bool bCanTakeItem = false;
	bool bCanOpenBox = false;

	// 상호작용 가능 변수
	UPROPERTY()
	bool bPlayerInRange = false;	

	const FItemData* ItemDataPtr;

};
