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
	// @TODO �κ��丮 ����
	
	if (Item.ItemID.IsValid() && CurrentCookSet && CurrentCookSet->bIsPlayerOverlapping)
	{
		// @TODO Create UI '�丮�ϱ� E' -> UI �����ϸ� bCanCook Ȱ��ȭ ��Ű��
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
		UE_LOG(LogTemp, Warning, TEXT("TryStartCooking: �丮 �Ұ��� ���°ų� ����� ����."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("�丮 ����!"));

	// �ִϸ��̼� ���, �κ��丮 ó�� �� �߰� ����	

	// �丮 UI ����

	// @TODO ���ִ� �ִϸ��̼� ���
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
	// @TODO ������ �ִϸ��̼�
	// @TODO �丮 ������ UI ����
}

void ATestCharacter::FailedCooking()
{
	// @TODO ������ �ִϸ��̼�
	// @TODO ���� UI ����
}

