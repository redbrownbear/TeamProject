// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

USTRUCT(BlueprintType)
struct FHandItem : public FTableRowBase
{
    GENERATED_BODY()

	// ������ ���� ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 1;
};

class ACookSet;
struct FRecipeData;

UCLASS()
class TEAMPROJECT_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    // �κ��丮���� ������ ���� �� �տ� ���
    void HoldItem(const FHandItem& Item);

    // �丮 ���� (�丮 Ű �Է� �� ȣ��)
    void StartCooking(const FHandItem& Item);

    // ĳ���Ͱ� ���� ������ ������ ����
    void RemoveHoldItem(const FHandItem& Item);

public:
    // �丮 ����
    void SuccessCooking();

    // �丮 ����
    void FailedCooking();

protected:
    // �տ� �� ������
    UPROPERTY()
    TArray<FHandItem> CurrentHeldItem;

    // �丮 ���
    UPROPERTY()
    ACookSet* CurrentCookSet = nullptr;

    // �丮 ���� ����
    UPROPERTY(BlueprintReadOnly)
    bool bCanCook = false;
};
