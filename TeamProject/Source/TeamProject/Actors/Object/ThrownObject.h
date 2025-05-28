// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrownObject.generated.h"

// this class is Only visible actor which will attached untile 
// monster throw this
// when monster throw this, it will be deleted, and monster will spawn Projectile

struct FItemData;
class IMonsterInterface;

UCLASS()
class TEAMPROJECT_API AThrownObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrownObject();

	virtual void SetDataWithHandle(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;


protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "ItemData"))
	FDataTableRowHandle DataTableRowHandle;
	const UDataTable* ItemDataTable;
	const FItemData* ItemTableRow;

public:
	void AttachToMonster(IMonsterInterface* Monster, FName SocketName);

};
