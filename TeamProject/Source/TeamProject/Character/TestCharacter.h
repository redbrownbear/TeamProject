// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

USTRUCT(BlueprintType)
struct FHandItem : public FTableRowBase
{
    GENERATED_BODY()

	// 아이템 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

    // 아이템 개수
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
    // 인벤토리에서 아이템 선택 → 손에 들기
    void HoldItem(const FHandItem& Item);

    // 요리 시작 (요리 키 입력 시 호출)
    void StartCooking(const FHandItem& Item);

    // 캐릭터가 가진 아이템 데이터 제거
    void RemoveHoldItem(const FHandItem& Item);

public:
    // 요리 성공
    void SuccessCooking();

    // 요리 실패
    void FailedCooking();

protected:
    // 손에 든 아이템
    UPROPERTY()
    TArray<FHandItem> CurrentHeldItem;

    // 요리 재료
    UPROPERTY()
    ACookSet* CurrentCookSet = nullptr;

    // 요리 가능 여부
    UPROPERTY(BlueprintReadOnly)
    bool bCanCook = false;
};
