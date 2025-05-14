#include "Tool/TestPC.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

void ATestPC::BeginPlay()
{
	Super::BeginPlay();

	// IMC ���
	if (APlayerController* PC = Cast<APlayerController>(this))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Inventory, 0); // �κ��丮 â
			Subsystem->AddMappingContext(IMC_Cook, 1);
		}
	}
}

void ATestPC::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInput);

	// �丮 ���
	EnhancedInput->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &ATestPC::OnPressedE);
	EnhancedInput->BindAction(StartCooking, ETriggerEvent::Triggered, this, &ATestPC::OnPressedQ);

}

void ATestPC::OnPressedE()
{
	ShowInventory(true);
}

void ATestPC::OnPressedQ()
{
	ATestCharacter* MyChar = Cast<ATestCharacter>(GetPawn());
	if (MyChar)
	{
		//MyChar->StartCooking();
	}
}

void ATestPC::ShowInventory(bool bShow)
{
	// Create Inventory UI 
}
