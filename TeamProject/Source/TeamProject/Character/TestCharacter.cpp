#include "Character/TestCharacter.h"
#include "Instrument/CookSet.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void ATestCharacter::HoldItem(const FHandItem& Item)
{
	// @TODO 인벤토리 연결
	
	if (Item.ItemID.IsValid() && CurrentCookSet && CurrentCookSet->bIsPlayerOverlapping)
	{
		// @TODO Create UI '요리하기 E' -> UI 생성하면 bCanCook 활성화 시키기
		if (bCanCook)
		{
			StartCooking(Item);
		}
	}
}

void ATestCharacter::StartCooking(const FHandItem& Item)
{
	if (!bCanCook || Item.ItemID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryStartCooking: 요리 불가능 상태거나 쿡셋이 없음."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("요리 시작!"));

	// 애니메이션 재생, 인벤토리 처리 등 추가 예정	

	// 요리 UI 끄기

	// @TODO 재료넣는 애니메이션 재생
	// RemoveInventoryItem();
	// RemoveHoldItem();

	RemoveHoldItem(Item);
}

void ATestCharacter::RemoveHoldItem(const FHandItem& Item)
{
	CurrentHeldItem.RemoveAll([&](const FHandItem& Held)
		{
			return Held.ItemID == Item.ItemID;
		});
}

void ATestCharacter::SuccessCooking()
{
	// @TODO 성공한 애니메이션
	// @TODO 요리 아이템 UI 생성
}

void ATestCharacter::FailedCooking()
{
	// @TODO 실패한 애니메이션
	// @TODO 실패 UI 생성
}

